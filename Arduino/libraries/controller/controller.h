#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Servo.h>
#include "sia_board_io.h"
#include "Adafruit_NeoPixel.h"
#include "hardware.h"

namespace controller {

// const
constexpr int maxLenkWinkel = 20, stopLenkWinkel = 90;
constexpr int BLINKER_LENGTH = 3;

// Millis
unsigned long previousSpeedMillis = 0;
unsigned long previousLenkMillis = 0;
unsigned long previousBlinkerMillis[] = {0, 0};

#ifdef CAR1
constexpr int maxVorSpeed = 13, stopSpeed = 90, maxRueckSpeed = 13;
#else
constexpr int maxVorSpeed = 13, stopSpeed = 75, maxRueckSpeed = 13;
#endif


constexpr int BESCHL_DELAY = 150;
constexpr int LENK_DELAY = 15;
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

// Git Objekt fpr LED-Streifen zurück (side = 0 => links |||| side = 1 => rechts)
Adafruit_NeoPixel* getSide(int side) {
  if (side == 0) {
    return &blinkerLinks;
  } else {
    return &blinkerRechts;
  }
}

// Wird aufgerufen zum Blinker einschalten
void blinkerAnschalten(int side) {
  blinkerAn[side] = true;
}

// Wird aufgerufen zum Blinker ausschalten
void blinkerAusschalten(int side) {
  blinkerAn[side] = false;
  blinkerPos[side] = 0;
  Adafruit_NeoPixel *strip = getSide(side);
  strip->fill(0x00);
  strip->show();
}

// Wenn die Lenkung halb auf eine Seite eingeschlagen hat, wird geblinkt
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

// Beide Blinker werden eingeschaltet
void warnblinkerAn() {
  warnBlinker = true;
  for (int i = 0; i < 2; i++) {
    blinkerAn[i] = true;
    blinkerPos[i] = 0;
    previousBlinkerMillis[i] = 0; // sync beide Blinker
  }
}

// Beide Blinker werden ausgeschaltet
void warnblinkerAus() {
  warnBlinker = false;
  for (int i = 0; i < 2; i++) {
    blinkerAusschalten(i);
  }
}
//---------Licht------------------/
void fahrtlichtAnschalten() { // Ist immer an => keine ausschaltfunktion
  vorneFahrtlicht.fill(0xFF);
  vorneFahrtlicht.show();

  hintenFahrtUndRueckFahrLicht.setBrightness(0x99);
  hintenFahrtUndRueckFahrLicht.setPixelColor(1, 0xFF, 0x00, 0x00);
  hintenFahrtUndRueckFahrLicht.setPixelColor(2, 0xFF, 0x00, 0x00);
  hintenFahrtUndRueckFahrLicht.show();
}

// Rückfahrlicht wird nur beim Rückwärtsfahren angemacht (sind die beiden äußeren LEDs des hinteren LED-Streifen)
void rueckfahrlichtAnschalten() {
  rueckfahrLichtAn = true;
  hintenFahrtUndRueckFahrLicht.setPixelColor(0, 0xDD, 0xDD, 0xDD);
  hintenFahrtUndRueckFahrLicht.setPixelColor(3, 0xDD, 0xDD, 0xDD);
  hintenFahrtUndRueckFahrLicht.show();
}

// Schaltet Rückfahrlicht aus
void rueckfahrlichtAusschalten() {
  rueckfahrLichtAn = false;
  hintenFahrtUndRueckFahrLicht.setPixelColor(0, 0x00, 0x00, 0x00);
  hintenFahrtUndRueckFahrLicht.setPixelColor(3, 0x00, 0x00, 0x00);
  hintenFahrtUndRueckFahrLicht.show();
}

//---------Servo------------------/
/* -----------------------
   | min:           max: |
   | -100 < geschw < 100 |
   -----------------------
*/
// Muss zum fahren aufgerufen werden 100 = max | 0 = stop | -100 = maxRück
void fahren(int geschw) {
	if (geschw < -100)
		geschw = -100;
	else if (geschw > 100)
		geschw = 100;
	
	reachSpeed = map(geschw, -100, 100, maxRueckSpeed*-1, maxVorSpeed);
}

// Muss aufgerufen werden zum anhalten
void anhalten() {
  reachSpeed = 0;
}

/* ---------------------
   | min:         max: |
   | -20 < winkel < 20 |
   ---------------------
*/
// Lenken -20 = links | 0 = gerade | 20 rechts
void lenken(int winkel) {
  if (winkel < maxLenkWinkel * -1)
    reachLenkWinkel = maxLenkWinkel * -1;
  else if (winkel > maxLenkWinkel)
    reachLenkWinkel = maxLenkWinkel;
  else
    reachLenkWinkel = winkel;
}

// Muss aufgerufen werden um Zweiachslenkung an-/auszuschakten
void zweiachslenkung(bool on) {
  zweiachsLenkung = on; 
}

//////////////////////////// INIT ///

// setzt alles zurück (hält an, etc.)
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
// Diese Funktionen üssen bei jedem Loop Durchlauf aufgerufen werden!

void loopBlinker() {
  for (int i = 0; i < 2; i++) {
    if (!blinkerAn[i]) // Wenn der Blinker nicht an ist...
      continue; // Zum Anfang der Schleife

    Adafruit_NeoPixel *strip = getSide(i);
    unsigned long currentMillis = millis();
    if (currentMillis - previousBlinkerMillis[i] > currentBlinkerDelay[i]) { // Blinker Animation (delay)
      previousBlinkerMillis[i] = currentMillis;
      currentBlinkerDelay[i] = (BLINKER_LENGTH - 1 == blinkerPos[i] ? 500 : 125); // Sollen länger leuchten, wenn alle leuchten

      if (blinkerPos[i] >= BLINKER_LENGTH) { // alle Blinker werden ausgeschaltet
        strip->fill(0x00);
        blinkerPos[i] = 0;
      } else { // Blinker werden nacheinander eingeschaltet
// Die Blinker sind bei den Autos unterschiedlich angebracht => unterschiedliche Blinkrichtung	  
#ifdef CAR1
        strip->setPixelColor((BLINKER_LENGTH - 1) - blinkerPos[i], 0xFF, 0x45, 0x00);
#else
        strip->setPixelColor(blinkerPos[i], 0xFF, 0x45, 0x00);
#endif
        blinkerPos[i]++; // nächstes LED am LED-Streifen welches angehen soll
      }
    }
    strip->show();
  }
}

void loopSpeed() {
  if (reachSpeed == currentSpeed) // Wenn die Geschwindigkeit welche erreicht werden soll, bereits erreicht ist:
    return;

  unsigned long currentMillis = millis();
  if (currentMillis - previousSpeedMillis > BESCHL_DELAY) { // Langsam schneller/langsamer werden
    previousSpeedMillis = currentMillis;

    if (reachSpeed > currentSpeed) { // schneller werden
      currentSpeed++;
    } else { // langsamer werden
      currentSpeed--;
    }
    motor.write(stopSpeed + currentSpeed);

    // Rückfahrlicht
    if (currentSpeed < 0 && !rueckfahrLichtAn) // Fährt Rückwärts
      rueckfahrlichtAnschalten();
    else if (currentSpeed >= 0 && rueckfahrLichtAn) // Fährt vorwärts
      rueckfahrlichtAusschalten();
  }
}

void loopLenken() {
  if (reachLenkWinkel == currentLenkWinkel) // Wenn der Lenkwinkel, welcher erreicht werden soll bereits erreicht ist.
    return;

  // KEIN lenken im Stand. (muss noch gemacht werden)
  /*if (currentSpeed > (int)(maxSpeed / 3) * -1 || currentSpeed < (int)(maxSpeed / 3))
    return;*/

  unsigned long currentMillis = millis();
  if (currentMillis - previousLenkMillis > LENK_DELAY) { // langsam lenken
    previousLenkMillis = currentMillis;

    if (reachLenkWinkel > currentLenkWinkel) { // vergrößern | rechts
      currentLenkWinkel++;
    } else { // links
      currentLenkWinkel--;
    }

    // Blinker bei entsprechendem Lenk-Winkel an bzw. ausschalten
    autoBlink();

    frontAxis.write(stopLenkWinkel + currentLenkWinkel);
    if (zweiachsLenkung) { // Hinterachse wird entgegengesetzt angesteuert
      hinterachseLenkWinkel = currentLenkWinkel * -1;
      rearAxis.write(stopLenkWinkel + hinterachseLenkWinkel);
    } else if (hinterachseLenkWinkel != 0) { // Wenn Zweiachslenkung ausgeschaltet wurde und die Hintere Achse noch schräg ist...

		// Wird wieder auf mittelstellung gebracht, wenn vorne auch gelenkt wird.
      if (hinterachseLenkWinkel > 0)
        hinterachseLenkWinkel--;
      else
        hinterachseLenkWinkel++;

      rearAxis.write(stopLenkWinkel + hinterachseLenkWinkel);
    }
  }

}

///////////////////////////////HAUPT-FUNKTIONEN////////////////////////////////////////////

// Wird aufgerufen und auf standartwerde gesetzt
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
}

// Ruft die 3 loopFunktionen auf. (Damit man nicht jede einzeln aufrufen muss)
void loopController() {
  loopBlinker();
  loopSpeed();
  loopLenken();
}
}
#endif
