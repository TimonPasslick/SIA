#ifndef HARDWARE_H
#define HARDWARE_H

#include <Servo.h>
#include "Adafruit_NeoPixel.h"
#include "radio_receiver.h"
#include "LiquidCrystal.h"

#if defined(CAR1) == defined (CAR2)
#error Definiere CAR1 oder CAR2!
#endif

// Servos
Servo motor, frontAxis, rearAxis;

// Blinker
Adafruit_NeoPixel blinkerLinks{3, 8, NEO_GRB}, blinkerRechts{3, 4, NEO_GRB};

// Fahrlichter
#ifdef CAR1
Adafruit_NeoPixel vorneFahrtlicht{4, 16, NEO_GRB};
Adafruit_NeoPixel hintenFahrtUndRueckFahrLicht{4, 17, NEO_GRB};
#else
Adafruit_NeoPixel hintenFahrtUndRueckFahrLicht{4, 20, NEO_GRB};
Adafruit_NeoPixel vorneFahrtlicht{4, 19, NEO_GRB};
#endif

// Fernsteuerung
radio_receiver controllerSignalReceiver;

// LCD
constexpr int rs = 31, en = 30, d4 = 29, d5 = 28, d6 = 27, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Buttons
#ifdef CAR1
  //23 ok, 24 down, 25 up
  constexpr int buttonPins[3] {23, 24, 25};
#endif

#endif
