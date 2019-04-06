// third party
#include "radio_receiver.h"

// siaLib
#include "define.h" // #defines RADIO_RECEIVER_NULL_CH_n, 0 <= n <= 9

radio_receiver controllerSignalReceiver;

void setup() {
  Serial.begin(9600);
  
  controllerSignalReceiver.init();
  
  delay(2000);
}

void loop() {
  const auto steerSignal = controllerSignalReceiver.AverageChannel(1); //- RADIO_RECEIVER_NULL_CH_1;
  Serial.print("steer: ");
  Serial.println(steerSignal);

  const auto speedSignal = controllerSignalReceiver.AverageChannel(2); //- RADIO_RECEIVER_NULL_CH_2;
  Serial.print("speed: ");
  Serial.println(speedSignal);
}