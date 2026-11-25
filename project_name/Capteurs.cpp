#include "Capteurs.h"

CapteurDistance::CapteurDistance(int trig, int echo)
    : Composant("Distance", trig),
      Capteur("Distance", trig),
      pinTrig_(trig),
      pinEcho_(echo),
      distance_(-1),
      correction_(0) {}

void CapteurDistance::lire() {
    digitalWrite(pinTrig_, LOW);
    delayMicroseconds(2);
    digitalWrite(pinTrig_, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrig_, LOW);

    const unsigned long duree = pulseIn(pinEcho_, HIGH, 30000);
    if (duree == 0) {
        distance_ = -1;
    } else {
        distance_ = duree * 0.0343f / 2.0f + correction_;
    }

    if (distance_ > 0) enregistrer(mesure());
}

Mesure CapteurDistance::mesure() const {
    return Mesure(distance_, "cm", millis());
}

String CapteurDistance::versLCD() const {
    if (distance_ < 0) return "Distance: erreur";
    return "Dist: " + String(distance_, 1) + "cm";
}

bool CapteurDistance::seuilDepasse(float seuil) const {
    return distance_ > 0 && distance_ < seuil;
}

void CapteurDistance::calibrer() {
    correction_ = 0;
}

CapteurNumerique::CapteurNumerique(String n, int p, bool activeLow)
    : Composant(n, p), Capteur(n, p),
      actifBas_(activeLow), etat_(false) {}

void CapteurNumerique::lire() {
    const int valeurPin = digitalRead(pin);
    etat_ = actifBas_ ? (valeurPin == LOW) : (valeurPin == HIGH);
    enregistrer(mesure());
}

Mesure CapteurNumerique::mesure() const {
    return Mesure(etat_ ? 1 : 0, "", millis());
}

String CapteurNumerique::versLCD() const {
    return nom + ": " + (etat_ ? "ACTIF" : "INACTIF");
}

bool CapteurNumerique::seuilDepasse(float /*seuil*/) const {
    return etat_;
}

CapteurHumidite::CapteurHumidite()
    : Composant("SHT31", -1), Capteur("SHT31", -1),
      humidite_(NAN), temperature_(NAN) {}

void CapteurHumidite::lire() {
    humidite_ = sht31.readHumidity();
    temperature_ = sht31.readTemperature();
    if (!isnan(humidite_)) enregistrer(mesure());
}

Mesure CapteurHumidite::mesure() const {
    return Mesure(humidite_, "%", millis());
}

String CapteurHumidite::versLCD() const {
    if (isnan(humidite_)) return "SHT31: erreur";
    return "Hum: " + String(humidite_, 1) + "%";
}

bool CapteurHumidite::seuilDepasse(float seuil) const {
    return !isnan(humidite_) && humidite_ > seuil;
}

void CapteurHumidite::calibrer() {
    // Aucune correction logicielle définie pour ce capteur.
}
