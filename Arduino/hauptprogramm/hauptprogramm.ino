#include "menu.h"

void setup() {
  
}

void loop() {
  if (Serial2.available())
    menu::smartphoneSignal(Serial2.read());
}
