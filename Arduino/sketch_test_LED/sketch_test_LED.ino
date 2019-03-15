//Arduino
#include <Servo.h>

//third party
#include "radio_receiver.h"

//siaLib
#include "define.h" //#defines RADIO_RECEIVER_NULL_CH_n, 0 <= n <= 9
#include "sia_board_io.h" //#defines pin names

void setup() {
  Serial.begin(9600);
  pinMode(d4,OUTPUT);

}
void loop() {
  for (int i = 0; i < 50; i++)
  {
  delay(100);
  digitalWrite(i);
  Serial.print(i);
  }
}
