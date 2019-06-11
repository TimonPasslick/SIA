#define DEBUG
#define CAR2
#include "debug_print.h"
#include <Servo.h>
#include "define.h"
#include "sia_board_io.h"
#include "LiquidCrystal.h"
#include "controller.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////

int vorneGross, vorneKlein, vorneLinks, vorneRechts;
int hintenGross, hintenKlein, hintenLinks, hintenRechts;

const int stoppGeschwindigkeit = 0;
const int maxGeschwindigkeit = 100;

enum { links, rechts };

///////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  controller::setupController();
  controller::zweiachslenkung(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  readSensor();
  writeSensor();
  if (vorneLinks >= 50 || vorneRechts >= 50)
  {
    lcd.setCursor(6, 0);
    lcd.print("Seite");
    switch (fahrtRichtung())
    {
      case links:
        {
          linksLenken();
          lcd.setCursor(6, 1);
          lcd.print("Links");
          break;
        }
      case rechts:
        {
          rechtsLenken();
          lcd.setCursor(6, 1);
          lcd.print("rechts");
          break;
        }
    }

  }
  else
  {
    lcd.setCursor(6, 0);
    lcd.print("Frei");
    controller::lenken(0);
  }
  if (vorneGross >= 110 || vorneKlein >= 150)
    stopp();
  else
    controller::fahren(100);
    debugPrintLine("Frei");

  controller::loopController();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void readSensor()
{
  vorneGross   = analogRead(A0);
  vorneKlein   = analogRead(A1);
  vorneLinks   = analogRead(A2);
  vorneRechts  = analogRead(A3);
  hintenGross  = analogRead(A4);
  hintenKlein  = analogRead(A5);
  hintenLinks  = analogRead(A6);
  hintenRechts = analogRead(A7);
}
void writeSensor()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(vorneGross);
  lcd.setCursor(13, 0);
  lcd.print(vorneKlein);
  lcd.setCursor(0, 1);
  lcd.print(vorneLinks);
  lcd.setCursor(13, 1);
  lcd.print(vorneRechts);
}
void anfahren()
{
  debugPrintLine("anfahren");
  for (int i = 0; i < maxGeschwindigkeit; i++)
  {
    motor.write(i);
    debugPrintLine(i);
  }
}
bool istObjektErkannt()
{
  debugPrintLine("Objekterkennung");
  if (vorneGross >= 200 /*|| vorneKlein >= 200*/ || vorneLinks >= 150 || vorneRechts >= 150)
  {
    debugPrint("true");
    return true;
  }
  else
  {
    debugPrint("false");
    return false;
  }
}
int fahrtRichtung()
{
  debugPrint("fahrtRichtung");
  return (vorneLinks >= vorneRechts) ? rechts : links;
}
void linksLenken()
{
  debugPrintLine("Links");
  controller::lenken(-20);
}
void rechtsLenken()
{
  debugPrintLine("rechts");
  controller::lenken(20);
}
void stopp()
{
  debugPrintLine("stopp");
  controller::fahren(0);
  controller::lenken(20);
}
