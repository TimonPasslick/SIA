#include "menu.h"

void setup() {
  Serial2.begin(9600);
  controllerSignalReceiver.init();
  lcd.begin(16, 2);
  Wire.begin();
  motor    .attach(SERVO_MC_ONE_OUT);
  frontAxis.attach(SERVO_TWO_OUT);
}

void loop() {
  while (Serial2.available())
    menu::smartphoneSignal(Serial2.read());
  //TODO: Knopf prüfen
}
