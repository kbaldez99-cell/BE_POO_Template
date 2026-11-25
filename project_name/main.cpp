#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include <Adafruit_SHT31.h>

#include "Configuration.h"
#include "Modeles.h"
#include "Capteurs.h"
#include "Buzzer.h"

rgb_lcd lcd;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

CapteurDistance capteurDistance(PIN_TRIG, PIN_ECHO);
CapteurNumerique microphone("Bruit", PIN_MICRO, true);
CapteurNumerique lumiere("Lumiere", PIN_LUMIERE, true);
CapteurHumidite capteurClimat;
Buzzer buzzer;
ModeAffichage mode;

bool alarmeArmee = false;
bool ancienTactile = false;
bool ancienBouton = HIGH;

unsigned long dernierAffichage = 0;
unsigned long derniereLectureSHT = 0;
unsigned long derniereAlerte = 0;

float seuilDistance = 100;
const float seuilHumidite = 80;

Alerte alertesActuelles = Alerte::Aucune;

void afficherLigne(int ligne, String texte) {
    lcd.setCursor(0, ligne);
    if (texte.length() > 16) texte = texte.substring(0, 16);
    lcd.print(texte);
    for (unsigned int i = texte.length(); i < 16; ++i) lcd.print(" ");
}

void gererTactile() {
    const bool etat = digitalRead(PIN_TACTILE);
    if (etat && !ancienTactile) {
        alarmeArmee = !alarmeArmee;
        lcd.clear();
        afficherLigne(0, alarmeArmee ? "Alarme ARMEE" : "Alarme OFF");
        delay(300);
    }
    ancienTactile = etat;
}

void gererBouton() {
    const bool etat = digitalRead(PIN_BOUTON);
    if (etat == LOW && ancienBouton == HIGH) {
        ++mode;
        lcd.clear();
        delay(200);
    }
    ancienBouton = etat;
}

void reglerSensibilite() {
    const int valeurPot = analogRead(PIN_POT);
    seuilDistance = map(valeurPot, 0, 1023, 20, 200);
}

void lireCapteurs() {
    capteurDistance.lire();
    microphone.lire();
    lumiere.lire();

    if (millis() - derniereLectureSHT >= 2000) {
        capteurClimat.lire();
        derniereLectureSHT = millis();
    }
}

void detecterAlertes() {
    alertesActuelles = Alerte::Aucune;
    if (!alarmeArmee) return;

    if (capteurDistance.seuilDepasse(seuilDistance))
        alertesActuelles |= Alerte::Mouvement;
    if (microphone.actif())
        alertesActuelles |= Alerte::Bruit;
    if (lumiere.actif())
        alertesActuelles |= Alerte::Lumiere;
    if (capteurClimat.seuilDepasse(seuilHumidite))
        alertesActuelles |= Alerte::Humidite;
}

void gererBuzzer() {
    if (!alarmeArmee) {
        buzzer.arreter();
        return;
    }
    if (millis() - derniereAlerte < 600) return;

    if ((alertesActuelles & Alerte::Mouvement) != Alerte::Aucune)
        buzzer.alerteMouvement();
    else if ((alertesActuelles & Alerte::Bruit) != Alerte::Aucune)
        buzzer.alerteBruit();
    else if ((alertesActuelles & Alerte::Lumiere) != Alerte::Aucune)
        buzzer.alerteLumiere();
    else if ((alertesActuelles & Alerte::Humidite) != Alerte::Aucune)
        buzzer.alerteHumidite();
    else
        buzzer.arreter();

    derniereAlerte = millis();
}

void afficherLCD() {
    if (millis() - dernierAffichage < 500) return;
    dernierAffichage = millis();

    switch (mode.valeur()) {
        case 0:
            afficherLigne(0, "SENTINELLE");
            afficherLigne(1, alarmeArmee ? "Surveillance ON" : "Surveillance OFF");
            break;

        case 1:
            afficherLigne(0, capteurDistance.versLCD());
            afficherLigne(1, "Seuil: " + String(seuilDistance, 0) + "cm");
            break;

        case 2:
            afficherLigne(0, microphone.versLCD());
            afficherLigne(1, lumiere.versLCD());
            break;

        case 3:
            afficherLigne(0, capteurClimat.versLCD());
            afficherLigne(1, "Temp: " + String(capteurClimat.getTemperature(), 1) + "C");
            break;
    }
}

void setup() {
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    pinMode(PIN_MICRO, INPUT);
    pinMode(PIN_LUMIERE, INPUT);
    pinMode(PIN_TACTILE, INPUT);
    pinMode(PIN_BOUTON, INPUT_PULLUP);
    pinMode(PIN_BUZZER, OUTPUT);

    digitalWrite(PIN_TRIG, LOW);
    noTone(PIN_BUZZER);

    Wire.begin(PIN_SDA, PIN_SCL);
    lcd.begin(16, 2);
    lcd.setRGB(255, 255, 255);

    afficherLigne(0, "SENTINELLE");
    afficherLigne(1, "Initialisation");

    if (!sht31.begin(SHT31_ADRESSE)) {
        afficherLigne(0, "Erreur SHT31");
        afficherLigne(1, "Verifier I2C");
        while (true) delay(100);
    }

    delay(1500);
    lcd.clear();
}

void loop() {
    gererTactile();
    gererBouton();
    reglerSensibilite();
    lireCapteurs();
    detecterAlertes();
    gererBuzzer();
    afficherLCD();
}
