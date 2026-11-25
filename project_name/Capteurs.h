#pragma once
#include <Adafruit_SHT31.h>
#include "Composants.h"

extern Adafruit_SHT31 sht31;

class CapteurDistance : public Capteur, public Affichable,
                        public Alertable, public Calibrable {
private:
    int pinTrig_;
    int pinEcho_;
    float distance_;
    float correction_;

public:
    CapteurDistance(int trig, int echo);
    void lire() override;
    Mesure mesure() const override;
    String versLCD() const override;
    bool seuilDepasse(float seuil) const override;
    void calibrer() override;
    float valeur() const { return distance_; }
};

class CapteurNumerique : public Capteur, public Affichable, public Alertable {
private:
    bool actifBas_;
    bool etat_;

public:
    CapteurNumerique(String n, int p, bool activeLow = true);
    void lire() override;
    Mesure mesure() const override;
    String versLCD() const override;
    bool seuilDepasse(float seuil) const override;
    bool actif() const { return etat_; }
};

class CapteurHumidite : public Capteur, public Affichable,
                        public Alertable, public Calibrable {
private:
    float humidite_;
    float temperature_;

public:
    CapteurHumidite();
    void lire() override;
    Mesure mesure() const override;
    String versLCD() const override;
    bool seuilDepasse(float seuil) const override;
    void calibrer() override;
    float getTemperature() const { return temperature_; }
    float getHumidite() const { return humidite_; }
};
