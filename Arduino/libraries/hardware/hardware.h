#ifndef HARDWARE_H
#define HARDWARE_H

#include <Servo.h>
#include "radio_receiver.h"
#include "LiquidCrystal.h"

Servo motor, frontAxis, rearAxis;

radio_receiver controllerSignalReceiver;

constexpr int rs = 31, en = 30, d4 = 29, d5 = 28, d6 = 27, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#endif
