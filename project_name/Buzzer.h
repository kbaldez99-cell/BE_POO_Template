#pragma once
#include "Composants.h"
#include "Configuration.h"

class Buzzer : public Composant {
public:
    Buzzer() : Composant("Buzzer", PIN_BUZZER) {}

    void activer(int frequence, int duree) { tone(pin, frequence, duree); }
    void arreter() { noTone(pin); }
    void alerteBruit() { activer(2200, 150); }
    void alerteMouvement() { activer(1200, 250); }
    void alerteLumiere() { activer(3000, 100); }
    void alerteHumidite() { activer(800, 400); }
};
