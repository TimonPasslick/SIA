#define DEBUG
#define CAR2
#include "debug_print.h"
#include <Servo.h>
#include "define.h"
#include "sia_board_io.h"
#include "LiquidCrystal.h"
#include "controller.h"

void getSensor();
int setupMotor();
void testFront(int Gross, int Klein);
void testSide(int Links, int Rechts);
void stopp();
int getRichtung();
void rechtsLenken();
void linksLenken();

int vorneGross, vorneKlein, vorneLinks, vorneRechts;
int hintenGross, hintenKlein, hintenLinks, hintenRechts;

int maxGeschwindigkeit = 100;
const int stoppGeschwindigkeit = 0;

enum { links, rechts };

void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  controller::setupController();
  controller::zweiachslenkung(true);
  getSensor();
  maxGeschwindigkeit = setupMotor();
}

void loop()
{
  getSensor();
  testFront(vorneGross,vorneKlein);
  testSide(vorneLinks,vorneRechts);
}

void getSensor()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  vorneLinks   = analogRead(A2);
  lcd.setCursor(5, 0);
  vorneKlein   = analogRead(A2);
  lcd.print(vorneKlein);
  lcd.setCursor(9, 0);
  vorneGross   = analogRead(A0);
  lcd.print(vorneGross);
  lcd.setCursor(13, 0);
  vorneRechts  = analogRead(A3);
  lcd.print(vorneRechts);
  lcd.setCursor(0, 1);
  hintenLinks  = analogRead(A6);
  lcd.print(hintenLinks);
  lcd.setCursor(5, 1);
  hintenKlein  = analogRead(A5);
  lcd.print(hintenKlein);
  lcd.setCursor(9, 1);
  hintenGross  = analogRead(A4);
  lcd.print(hintenGross);
  lcd.setCursor(13, 1);
  hintenRechts = analogRead(A7);
  lcd.print(hintenRechts);
}

int setupMotor()
{
  int Abstand = hintenGross;
  int Geschwindigkeit = 70;
  while (Abstand + 100 >= hintenGross)
  {
    Geschwindigkeit++;
    controller::fahren(Geschwindigkeit);
    delay (100);
    getSensor();
  }
  return Geschwindigkeit;
}

void testFront(int Gross, int Klein)
{
  if (Gross >= 110 || Klein >= 150)
  {
    stopp();
    debugPrintLine("stopp");
  }
  else
  {
    controller::fahren(maxGeschwindigkeit);
    debugPrintLine("-");
  }
}

void testSide(int Links, int Rechts)
{
  if (Links >= 50 || Rechts >= 50)
  {
    switch (getRichtung())
    {
      case rechts:
        {
          rechtsLenken();
          break;
        }
      case links:
        {
          linksLenken();
          break;
        }
    }
  }
  else
  {
    controller::lenken(0);
    debugPrintLine("-");
  }
}

void stopp()
{
  debugPrintLine("stopp");
  controller::fahren(0);
  testSide(hintenLinks,hintenRechts);
}

int getRichtung()
{
  return (vorneLinks >= vorneRechts) ? rechts : links;
}

void rechtsLenken()
{
  debugPrintLine("rechts");
  controller::lenken(20);
}

void linksLenken()
{
  debugPrintLine("links");
  controller::lenken(-20);
}
