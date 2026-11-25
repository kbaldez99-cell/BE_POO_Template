#include "Modeles.h"

Print& operator<<(Print& sortie, const Mesure& m) {
    sortie.print(m.valeur());
    sortie.print(" ");
    sortie.print(m.unite());
    return sortie;
}

Alerte operator|(Alerte a, Alerte b) {
    return static_cast<Alerte>(
        static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
    );
}

Alerte operator&(Alerte a, Alerte b) {
    return static_cast<Alerte>(
        static_cast<uint8_t>(a) & static_cast<uint8_t>(b)
    );
}

Alerte& operator|=(Alerte& a, Alerte b) {
    a = a | b;
    return a;
}
