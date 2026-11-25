#pragma once
#include <Arduino.h>
#include "Modeles.h"

class Composant {
protected:
    String nom;
    int pin;

public:
    Composant(String n, int p) : nom(n), pin(p) {}
    virtual ~Composant() = default;

    String getNom() const { return nom; }
    int getPin() const { return pin; }
};

class Affichable {
public:
    virtual String versLCD() const = 0;
    virtual ~Affichable() = default;
};

class Alertable {
public:
    virtual bool seuilDepasse(float seuil) const = 0;
    virtual ~Alertable() = default;
};

class Calibrable {
public:
    virtual void calibrer() = 0;
    virtual ~Calibrable() = default;
};

class Capteur : virtual public Composant, public Journalisable {
public:
    Capteur(String n, int p) : Composant(n, p) {}
    virtual void lire() = 0;
    virtual Mesure mesure() const = 0;
    virtual ~Capteur() = default;
};
