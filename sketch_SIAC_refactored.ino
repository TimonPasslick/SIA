//Arduino
#include <Servo.h>

//third party
#include "radio_receiver.h"

//siaLib
#include "define.h" //#defines RADIO_RECEIVER_NULL_CH_n, 0 <= n <= 9
#include "sia_board_io.h" //#defines pin names


Servo motor;
Servo frontAxis, rearAxis;

radio_receiver controllerSignalReceiver;

/////////NEEDS SOME FINETUNING/////////
constexpr int
steerSignalMaxPossible = 50,
steerSignalMaxAllowed = 20;


constexpr int
speedSignalMinPossible = -50,
speedSignalMinAllowed = -50,

speedSignalMaxPossible = 50,
speedSignalMaxAllowed = 50,

stopSpeed = 75;
///////////////////////////////////////


void setup() {
  motor    .attach(SERVO_MC_ONE_OUT);
  frontAxis.attach(SERVO_TWO_OUT);
  rearAxis .attach(SERVO_ONE_OUT);

  controllerSignalReceiver.init();
}

//Running the loop only 25 times a second is responsive enough and saves battery.
void loop25Hz();

void loop() {
  constexpr int loopTime = 1000 / 25;

  const auto start = millis();

  loop25Hz();

  const auto used = millis() - start;

  if (used < loopTime)
    delay(loopTime - used);
}

int limit(int value, const int maxPossible, const int maxAllowed) {
  value = map(value, 0, maxPossible, 0, maxAllowed);

  if (value > maxAllowed)
    value = maxAllowed;

  return value;
}

void loop25Hz() {
  //channel number is arbitrary choice: all channels are 0 if controller is switched of
  const bool off = controllerSignalReceiver.AverageChannel(0) < 20;

  //STEER
  {
    int steerSignal = controllerSignalReceiver.AverageChannel(1) - RADIO_RECEIVER_NULL_CH_1;

    steerSignal = limit(steerSignal, steerSignalMaxPossible, steerSignalMaxAllowed);

    if (off)
      steerSignal = 0;

    frontAxis.write(90 - steerSignal);
    rearAxis .write(90 + steerSignal);
  }

  //SPEED
  {
    int speedSignal = controllerSignalReceiver.AverageChannel(2) - RADIO_RECEIVER_NULL_CH_2;

    speedSignal = -speedSignal; //controller or servo is strange

    if (speedSignal > 0)
      speedSignal = limit(speedSignal, speedSignalMaxPossible, speedSignalMaxAllowed);
    if (speedSignal < 0)
      speedSignal = -limit(-speedSignal, -speedSignalMinPossible, -speedSignalMinAllowed);

    { //stop check
      static int prevSpeedSignal = 0;

      const bool switchingDirection = (prevSpeedSignal > 0 and speedSignal < 0) or
                                      (prevSpeedSignal < 0 and speedSignal > 0);

      if (switchingDirection or off)
        speedSignal = 0;

      prevSpeedSignal = speedSignal;
    } //stop check end

    motor.write(speedSignal + stopSpeed);
  }
}