#pragma once
#include <Arduino.h>

// Configuration matérielle prévue pour une carte ESP8266 NodeMCU.
#define PIN_SDA D2
#define PIN_SCL D1

#define PIN_TRIG D6
#define PIN_ECHO D7

#define PIN_MICRO D0
#define PIN_LUMIERE 1       // GPIO1 / TX : utiliser cette broche peut gêner le port série.

#define PIN_TACTILE D8
#define PIN_BOUTON D3

#define PIN_BUZZER D5
#define PIN_POT A0

constexpr uint8_t LCD_ADRESSE = 0x3E;
constexpr uint8_t SHT31_ADRESSE = 0x44;
