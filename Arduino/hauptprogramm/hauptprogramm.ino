#include "menu.h"

#include "hardware.h"

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
  Serial2.write(0x00); //Bedeutung: Neue Eingaben werden wieder sofort verarbeitet.
  //TODO: Knopf prüfen
}
