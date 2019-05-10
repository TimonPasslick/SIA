#define CAR1

#include "menu.h"

void setup() {
  controllerSignalReceiver.init();
  motor    .attach(SERVO_MC_ONE_OUT);
  frontAxis.attach(SERVO_ONE_OUT);
  rearAxis .attach(SERVO_TWO_OUT);

  #ifdef CAR1
    lcd.begin(16, 2);
    for (const int pin : buttonPins)
      pinMode(pin, INPUT);
    menu::enter();
  #endif
  
  #ifdef CAR2
    Serial2.begin(9600);
    Wire.begin();
  #endif
}

void loop() {
  #ifdef CAR2
    if (Serial2.available()) {
      Serial2.write('b'); //Bedeutung: Neue Eingaben werden mit 40 Byte gepuffert und nicht sofort verarbeitet. ("busy")
      while (Serial2.available())
        menu::smartphoneSignal(Serial2.read());
      Serial2.write('r'); //Bedeutung: Neue Eingaben werden wieder sofort verarbeitet. ("ready")
    }
  #endif

  #ifdef CAR1
    static bool buttonsPressed[3] {false, false, false};
  
    for (int idx = 0; idx < 3; ++idx) {
      const bool buttonPressedUpdate = digitalRead(buttonPins[idx]) == HIGH;
      if (buttonPressedUpdate and not buttonsPressed[idx]) {
        switch (buttonPins[idx]) {
          case 23:
            menu::ok();
            break;
          case 24:
            menu::down();
            break;
          case 25:
            menu::up();
            break;
        }
      }
      buttonsPressed[idx] = buttonPressedUpdate;
    }
  #endif
}
