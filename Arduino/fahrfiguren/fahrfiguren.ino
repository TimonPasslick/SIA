#define CAR2

#include "menu.h"
#include "controller.h"

namespace ellipse {
  constexpr double totalMicros = 2.5 * 1000 * 1000;
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
    return true;
  }
}

void setup() {
  control::setupController();
  control::reset();
  motor.write(80);
}

void loop() {
  //MOD_ENTER(ellipse)
}
