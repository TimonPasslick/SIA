#include <Servo.h>
#include "sia_board_io.h"
#include "Adafruit_NeoPixel.h"
#include "hardware.h"

namespace control {

// const
constexpr int maxSpeed = 13, stopSpeed = 90;
constexpr int maxLenkWinkel = 20, stopLenkWinkel = 90;
constexpr int BLINKER_LENGTH = 3;

// Millis
unsigned long previousSpeedMillis = 0;
unsigned long previousLenkMillis = 0;
unsigned long previousBlinkerMillis[] = {0, 0};

// Delays
constexpr int BESCHL_DELAY = 150;
constexpr int LENK_DELAY = 30;
constexpr int STD_BLINKER_DELAY = 125;

// Enums
enum {VORWAERTS = 0, RUECKWAERTS = 1};
enum {LINKS = 0, RECHTS = 1};

// Blinker
bool warnBlinker = false;
bool blinkerAn[] = {false, false};
int blinkerPos[] = {0, 0};
int currentBlinkerDelay[] = {0, 0};

// Licht
bool rueckfahrLichtAn = false;

// Speed
int currentSpeed = 0;
int reachSpeed = 0;

// Lenkung
bool zweiachsLenkung = false;
int hinterachseLenkWinkel = 0;
int currentLenkWinkel = 0;
int reachLenkWinkel = 0;

//////////////////////////// HILFS-FUNKTIONEN/////////////////////////////////////

Adafruit_NeoPixel* getSide(int side) {
  if (side == 0) {
    return &blinkerLinks;
  } else {
    return &blinkerRechts;
  }
}

void blinkerAnschalten(int side) {
  blinkerAn[side] = true;
}

void blinkerAusschalten(int side) {
  blinkerAn[side] = false;
  blinkerPos[side] = 0;
  Adafruit_NeoPixel *strip = getSide(side);
  strip->fill(0x00);
  strip->show();
}

void autoBlink() {
  // Wenn die Warnblinker an ist
  if (warnBlinker)
    return;

  if (currentLenkWinkel <= (int)(maxLenkWinkel / 2) * -1 && !blinkerAn[LINKS]) { // --- LINKS
    blinkerAnschalten(LINKS);
    // Sicherheitshalber anderen Blinker ausschalten, falls an...
    blinkerAusschalten(RECHTS);
  } else if (currentLenkWinkel > (int)(maxLenkWinkel / 2) * -1 && blinkerAn[LINKS]) {
    blinkerAusschalten(LINKS);
  }

  if (currentLenkWinkel >= (int)(maxLenkWinkel / 2) && !blinkerAn[RECHTS]) { // --- RECHTS
    blinkerAnschalten(RECHTS);
    // Sicherheitshalber anderen Blinker ausschalten, falls an...
    blinkerAusschalten(LINKS);
  } else if (currentLenkWinkel < (int)(maxLenkWinkel / 2) && blinkerAn[RECHTS]) {
    blinkerAusschalten(RECHTS);
  }
}

/////////////////////////////AUFGERUFENE-FUNKTIONEN//////////////////////////////////////

void warnblinkerAn() {
  warnBlinker = true;
  for (int i = 0; i < 2; i++) {
    blinkerAn[i] = true;
    blinkerPos[i] = 0;
    previousBlinkerMillis[i] = 0; // sync beide Blinker
  }
}

void warnblinkerAus() {
  warnBlinker = false;
  for (int i = 0; i < 2; i++) {
    blinkerAusschalten(i);
  }
}
//---------Licht------------------/
void fahrtlichtAnschalten() { // Ist immer an => keine ausschaltfunktion....?
  vorneFahrtlicht.fill(0xFF);
  vorneFahrtlicht.show();

  hintenFahrtUndRueckFahrLicht.setBrightness(0x99);
  hintenFahrtUndRueckFahrLicht.setPixelColor(1, 0xFF, 0x00, 0x00);
  hintenFahrtUndRueckFahrLicht.setPixelColor(2, 0xFF, 0x00, 0x00);
  hintenFahrtUndRueckFahrLicht.show();
}

void rueckfahrlichtAnschalten() {
  rueckfahrLichtAn = true;
  hintenFahrtUndRueckFahrLicht.setPixelColor(0, 0xDD, 0xDD, 0xDD);
  hintenFahrtUndRueckFahrLicht.setPixelColor(3, 0xDD, 0xDD, 0xDD);
  hintenFahrtUndRueckFahrLicht.show();
}

void rueckfahrlichtAusschalten() {
  rueckfahrLichtAn = false;
  hintenFahrtUndRueckFahrLicht.setPixelColor(0, 0x00, 0x00, 0x00);
  hintenFahrtUndRueckFahrLicht.setPixelColor(3, 0x00, 0x00, 0x00);
  hintenFahrtUndRueckFahrLicht.show();
}

//---------Servo------------------/
/* ---------------------
   | min:         max: |
   | -13 < geschw < 13 |
   ---------------------
*/
void fahren(int geschw) {
  if (geschw < maxSpeed * -1)
    reachSpeed = maxSpeed * -1;
  else if (geschw > maxSpeed)
    reachSpeed = maxSpeed;
  else
    reachSpeed = geschw;
}

void anhalten() {
  reachSpeed = 0;
}

/* ---------------------
   | min:         max: |
   | -20 < winkel < 20 |
   ---------------------
*/
void lenken(int winkel) {
  if (winkel < maxLenkWinkel * -1)
    reachLenkWinkel = maxLenkWinkel * -1;
  else if (winkel > maxLenkWinkel)
    reachLenkWinkel = maxLenkWinkel;
  else
    reachLenkWinkel = winkel;
}

void zweiachslenkung(bool on) {
  zweiachsLenkung = on; // TODO: Wenn manes ausschaltet, sollte die hintere Achse wieder gerade sein... aber auch nicht Lnken im stand.....
}

//////////////////////////// INIT ///

void reset() {
  // Blinker ausschalten
  for (int i = 0; i < 2; i++) {
    blinkerAusschalten(i);
  }

  // Geschwindigkeit
  anhalten();

  // Lenkung
  lenken(0);

  // Zweiachslenkung
  zweiachslenkung(false);
}

////////////////////////////LOOP-FUNKTIONEN////////////////////////////////

void loopBlinker() {
  for (int i = 0; i < 2; i++) {
    if (!blinkerAn[i])
      continue;

    Adafruit_NeoPixel *strip = getSide(i);
    unsigned long currentMillis = millis();
    if (currentMillis - previousBlinkerMillis[i] > currentBlinkerDelay[i]) {
      previousBlinkerMillis[i] = currentMillis;
      currentBlinkerDelay[i] = (BLINKER_LENGTH - 1 == blinkerPos[i] ? 500 : 125);

      if (blinkerPos[i] >= BLINKER_LENGTH) { // alle Blinker werden ausgeschaltet
        strip->fill(0x00);
        blinkerPos[i] = 0;
      } else { // Blinker werden nacheinander eingeschaltet
#ifdef CAR2
        strip->setPixelColor((BLINKER_LENGTH - 1) - blinkerPos[i], 0xFF, 0x45, 0x00);
#else
        strip->setPixelColor(blinkerPos[i], 0xFF, 0x45, 0x00);
#endif
        blinkerPos[i]++;
      }
    }
    strip->show();
  }
}

void loopSpeed() {
  if (reachSpeed == currentSpeed)
    return;

  unsigned long currentMillis = millis();
  if (currentMillis - previousSpeedMillis > BESCHL_DELAY) {
    previousSpeedMillis = currentMillis;

    if (reachSpeed > currentSpeed) { // vergrößern
      currentSpeed++;
    } else {
      currentSpeed--;
    }
    motor.write(stopSpeed + currentSpeed);

    // Rückfahrlicht
    if (currentSpeed < 0 && !rueckfahrLichtAn)
      rueckfahrlichtAnschalten();
    else if (currentSpeed >= 0 && rueckfahrLichtAn)
      rueckfahrlichtAusschalten();
  }
}

void loopLenken() {
  if (reachLenkWinkel == currentLenkWinkel)
    return;

  // KEIN lenken im Stand.
  if (currentSpeed > (int)(maxSpeed / 3) * -1 || currentSpeed < (int)(maxSpeed / 3))
    return;

  unsigned long currentMillis = millis();
  if (currentMillis - previousLenkMillis > LENK_DELAY) {
    previousLenkMillis = currentMillis;

    if (reachLenkWinkel > currentLenkWinkel) { // vergrößern | rechts
      currentLenkWinkel++;
    } else { // links
      currentLenkWinkel--;
    }

    // Blinker bei entsprechendem Lenk-Winkel an bzw. ausschalten
    autoBlink();

    frontAxis.write(stopLenkWinkel + currentLenkWinkel);
    if (zweiachsLenkung) {
      hinterachseLenkWinkel = currentLenkWinkel * -1;
      rearAxis.write(stopLenkWinkel + hinterachseLenkWinkel);
    } else if (hinterachseLenkWinkel != 0) { // Wenn Zweiachslenkung ausgeschaltet wurde und die Hintere Achse noch schräg ist...

      if (hinterachseLenkWinkel > 0)
        hinterachseLenkWinkel--;
      else
        hinterachseLenkWinkel++;

      rearAxis.write(stopLenkWinkel + hinterachseLenkWinkel);
    }
  }

}

///////////////////////////////HAUPT-FUNKTIONEN////////////////////////////////////////////

unsigned long pm;
int mode;

void setupController() {
  Serial.begin(9600);

  // Servos
  motor.attach(SERVO_MC_ONE_OUT); // speed
  frontAxis.attach(SERVO_ONE_OUT); // vorne
  rearAxis.attach(SERVO_TWO_OUT); // hinten

  // Blinker
  blinkerRechts.begin();
  blinkerLinks.begin();

  // Licht
  vorneFahrtlicht.begin();
  hintenFahrtUndRueckFahrLicht.begin();
  fahrtlichtAnschalten();

  // ---
  delay(100);
  reset(); // Standart-Werte werden initialisiert
  delay(100);
  for (int i = 0; i < 25; i++) {
    loopBlinker();
    loopSpeed();
    loopLenken();
  }
  delay(100);
  // ---

  //test
  mode = 0;

  fahren(0);
}

void loopController() {

  // Testen...
  unsigned long m = millis();
  /*if (m - pm > 5000) {
    pm = m;

    Serial.print("Mode: ");
    Serial.println(mode);

    if (mode == 0)
      fahren(13);
    else if (mode == 1)
      lenken(-20);
    else if (mode == 2)
      lenken(0);
    else if (mode == 3)
      anhalten();
    else if (mode == 4)
      lenken(-20);
    else if (mode == 5)
      fahren(13);
    else if (mode == 6)
      lenken(0);
    else if (mode == 7)
      anhalten();
    else if (mode == 8)
      fahren(-13);
    else if (mode == 9) {
      anhalten();
      warnblinkerAn();
      zweiachslenkung(true);
    }
    else if (mode == 10) {
      fahren(13);
      warnblinkerAus();
    } else if (mode == 11)
      lenken(-20);
    else if (mode == 12)
      lenken(0);
    else if (mode == 13)
      anhalten();
    else if (mode == 14)
      lenken(-20);
    else if (mode == 15)
      fahren(13);
    else if (mode == 16)
      lenken(0);
    else if (mode == 17)
      anhalten();
    else if (mode == 18)
      fahren(-13);
    else if (mode == 19)
      anhalten();
    else if (mode == 20)
      zweiachslenkung(false);
    else if (mode == 21)
      fahren(100);
    else if (mode == 22)
      anhalten();
    else if (mode == 23)
      warnblinkerAn();
    else if (mode == 24)
      lenken(-20);
    else if (mode == 25)
      fahren(1123);
    else if (mode == 26)
      anhalten();


    mode++;
  }*/

  loopBlinker();
  loopSpeed();
  loopLenken();
}
}