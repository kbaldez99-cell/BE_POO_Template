#pragma once
#include <Arduino.h>

// Une mesure contient une valeur, une unité et son horodatage.
class Mesure {
private:
    float valeur_;
    String unite_;
    unsigned long temps_;

public:
    Mesure(float valeur = 0, String unite = "", unsigned long temps = 0)
        : valeur_(valeur), unite_(unite), temps_(temps) {}

    float valeur() const { return valeur_; }
    String unite() const { return unite_; }
    unsigned long temps() const { return temps_; }

    bool operator<(float seuil) const { return valeur_ < seuil; }
    bool operator>(float seuil) const { return valeur_ > seuil; }
    bool operator==(const Mesure& m) const { return valeur_ == m.valeur_; }

    Mesure operator+(const Mesure& m) const {
        return Mesure(valeur_ + m.valeur_, unite_, millis());
    }

    Mesure operator-(const Mesure& m) const {
        return Mesure(valeur_ - m.valeur_, unite_, millis());
    }

    Mesure& operator+=(const Mesure& m) {
        valeur_ += m.valeur_;
        return *this;
    }

    operator float() const { return valeur_; }
};

Print& operator<<(Print& sortie, const Mesure& m);

class Historique {
private:
    static const int TAILLE = 10;
    Mesure mesures[TAILLE];
    int indice = 0;
    int nombre = 0;

public:
    Historique& operator<<(const Mesure& m) {
        mesures[indice] = m;
        indice = (indice + 1) % TAILLE;
        if (nombre < TAILLE) ++nombre;
        return *this;
    }

    // n = 0 correspond à la mesure la plus récente.
    Mesure operator[](int n) const {
        if (n < 0 || n >= nombre) return Mesure();
        const int position = (indice - 1 - n + TAILLE) % TAILLE;
        return mesures[position];
    }

    int taille() const { return nombre; }
};

class Journalisable {
protected:
    Historique historique;

public:
    void enregistrer(const Mesure& m) { historique << m; }
    Mesure derniereMesure() const { return historique[0]; }
    Mesure mesurePrecedente() const { return historique[1]; }
};

class Seuil {
private:
    float limite_;

public:
    explicit Seuil(float s = 0) : limite_(s) {}
    void regler(float s) { limite_ = s; }
    bool operator()(const Mesure& m) const { return m > limite_; }
};

enum class Alerte : uint8_t {
    Aucune = 0,
    Bruit = 1,
    Mouvement = 2,
    Lumiere = 4,
    Humidite = 8
};

Alerte operator|(Alerte a, Alerte b);
Alerte operator&(Alerte a, Alerte b);
Alerte& operator|=(Alerte& a, Alerte b);

class ModeAffichage {
private:
    int page_ = 0;

public:
    void operator++() {
        ++page_;
        if (page_ > 3) page_ = 0;
    }
    int valeur() const { return page_; }
};
