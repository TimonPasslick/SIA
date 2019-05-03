#ifndef FERNSTEUERUNG_H
#define FERNSTEUERUNG_H

#include "hardware.h"

//siaLib
#include "define.h" //#defines RADIO_RECEIVER_NULL_CH_n, 0 <= n <= 9
#include "sia_board_io.h" //#defines pin names

namespace fernsteuerung {

/////////NEEDS SOME FINETUNING/////////
constexpr int
maxSpeed = 13,
stopSpeed = 90,
switchTimeMs = 100;
///////////////////////////////////////


void _setup() {
}

int channel(const uint8_t number) {
  constexpr int nullChannels[10] {
    RADIO_RECEIVER_NULL_CH_0,
    RADIO_RECEIVER_NULL_CH_1,
    RADIO_RECEIVER_NULL_CH_2,
    RADIO_RECEIVER_NULL_CH_3,
    RADIO_RECEIVER_NULL_CH_4,
    RADIO_RECEIVER_NULL_CH_5,
    RADIO_RECEIVER_NULL_CH_6,
    RADIO_RECEIVER_NULL_CH_7,
    RADIO_RECEIVER_NULL_CH_8,
    RADIO_RECEIVER_NULL_CH_9,
  };

  return controllerSignalReceiver.AverageChannel(number) - nullChannels[number];
}

int limitChannel(const uint8_t number, const int maxAllowed) {
  return min(
           maxAllowed,
           map(
             channel(number),
             0, 100,
             0, maxAllowed
           )
         );
}

bool _loop() {
  //channel number is arbitrary choice: all channels are 0 if controller is switched off
  const bool off = controllerSignalReceiver.AverageChannel(0) < 15;

  //STEER
  {
    int steerAngle = limitChannel(1, 20);

    if (off)
      steerAngle = 0;

    frontAxis.write(90 - steerAngle);

    //if switch at left top of controller is off, don't steer with rear axis
    if (channel(5) < 100 / 2)
      steerAngle = 0;

    rearAxis.write(90 + steerAngle);
  }

  //SPEED
  {
    int motorSpeed = limitChannel(2, maxSpeed);

    if (motorSpeed < 0) {
      motorSpeed /= 3; //6kmh/2kmh
    }

    //servo is strange
    motorSpeed = -motorSpeed;

    static int prevSpeed = 0;

    const bool switchingDirection = (prevSpeed > 0 and motorSpeed < 0) or
                                    (prevSpeed < 0 and motorSpeed > 0);

    if (switchingDirection or off)
      motorSpeed = 0;
    
    prevSpeed = motorSpeed;

    motor.write(motorSpeed + stopSpeed);

    //steering not possible while switching direction but switchTimeMs should be small
    if (switchingDirection)
      delay(switchTimeMs);
  }
  return true; //Abbruch im Moment nicht möglich
}
}

#endif
