#define CAR1

#include "hardware.h"
#include "sia_board_io.h"

namespace ellipse {
  constexpr double totalMicros = 7.5 * 1000 * 1000;
  int startMicros;
  void _setup() {
    startMicros = micros();
  }
  bool _loop() {
    const double passedMicros = micros() - startMicros;
    const double progress = passedMicros / totalMicros;
    /*if (progress >= 1)
      return false;*/
    const double angleDegrees = 20 * sin(2 * PI * progress);
    frontAxis.write(90 + angleDegrees);
    rearAxis.write(90 - angleDegrees);
    Serial.print("angleDegrees: ");
    Serial.println(angleDegrees);
    return true;
  }
}

void setup() {
  motor    .attach(SERVO_MC_ONE_OUT);
  frontAxis.attach(SERVO_ONE_OUT);
  rearAxis .attach(SERVO_TWO_OUT);
  
  motor.write(78);  
  ellipse::_setup();

  Serial.begin(9600);
}

void loop() {
  ellipse::_loop();
}
