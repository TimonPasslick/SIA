// Arduino
#include <Servo.h>

// third party
#include "DistanceGP2Y0A21YK.h"
#include "radio_receiver.h"

// siaLib
#include "define.h" // #defines RADIO_RECEIVER_NULL_CH_n, 0 <= n <= 9
#include "sia_board_io.h" // #defines pin names


Servo motor;
Servo frontAxis, rearAxis;

DistanceGP2Y0A21YK  distanceSensors[4];

radio_receiver controllerSignalReceiver;


constexpr int motorSpeedStop = 75;


void setup() {
  motor.attach(SERVO_MC_ONE_OUT);
  motor.write(motorSpeedStop);

  frontAxis.attach(SERVO_TWO_OUT);
  frontAxis.write(90);

  rearAxis.attach(SERVO_ONE_OUT);
  rearAxis.write(90);

  distanceSensors[0].begin(ANALOG_IN_0);
  distanceSensors[1].begin(ANALOG_IN_1);
  distanceSensors[2].begin(ANALOG_IN_2);
  distanceSensors[3].begin(ANALOG_IN_3);

  controllerSignalReceiver.init();

  delay(1000);
}


void loop() {
  // STEER
  {
    const int steerSignal =  controllerSignalReceiver.AverageChannel(1) - RADIO_RECEIVER_NULL_CH_1;

    frontAxis.write(90 - 2 * steerSignal);
    rearAxis .write(90 + 2 * steerSignal);
  }

  // SPEED
  {
    int speedSignal = controllerSignalReceiver.AverageChannel(2) - RADIO_RECEIVER_NULL_CH_2;

    if (speedSignal > 90)
      speedSignal = 80;

    int motorSpeed = (2 * speedSignal) + 90;

    { // stop check
      static int prevSpeedSignal = 0;

      const bool switchingDirection = (prevSpeedSignal >= 0 and speedSignal < 0) or
                                      (prevSpeedSignal <= 0 and speedSignal > 0);

      bool sensorsTooClose = false;
      for (auto& sensor : distanceSensors) {
        const int distance = sensor.getDistanceCentimeter();
        if (distance >= 0 and distance < 15)
          sensorsTooClose = true;
      }

      if (switchingDirection or sensorsTooClose)
        motorSpeed = motorSpeedStop;

      prevSpeedSignal = speedSignal;
    } //stop check end

    motor.write(motorSpeed);
  }
}
