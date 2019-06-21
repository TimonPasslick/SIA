#define CAR2

#include <Servo.h>
#include "LiquidCrystal.h"
#include "sia_board_io.h"
#include "controller.h"
#include "hardware.h"
#include "warnbake_mega.h"

enum
{
  NOTHING = 0,
  LCD_CONTROLL,
  LED_SHIELD_NEW_ANIM,
  LED_SHIELD_STREAM_ANIM,
  LED_SHIELD_UPLOAD_ANIM,
  FERNSTEUERUNG,
  SENSOREN,
  TERMINAL,
  PIN_MODE,
  MENU,
  CARD_MODES_END
};
int m_CardMode = 0;

using namespace control;

void setup()
{
  controller::setupController();
  
  Serial.begin(9600); // Serieller Monitor
  Serial2.begin(9600); // Serielle Schnittstelle BluetoothModul

  // LCD Test
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suche Blueooth");
  lcd.setCursor(2, 1);
  lcd.print("Verbindung...");
  Serial.println("Bluetooth Verbindung wird gesucht...");

  setupController();
}

// Fehlermeldung, welche auf dem LCD ausgegeben wird
void error(String msg) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ERROR");
  lcd.setCursor(0, 1);
  lcd.print(msg);
}

// Es wird angezeigt, welche kachel von der AndroidApp ausgewählt wurde
void displayMode(String mode) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MODE");
  lcd.setCursor(0, 1);
  lcd.print(mode);
}

/*
   Commands müssen zwischen zwei spitzen Klammern geschrieben werden (AndroidCode) <COMMAND>
   Commands mit
*/
bool m_isCmd = false;
bool m_isCardClicked = false;
bool m_isConnected = false;
bool m_wasAllreadyConnected = false;
// Bytes welche das Bluetooth-Modul empfangen hat
String receiveCmd() {
  String cmd = "";

  while (Serial2.available() > 0) { // Solang es noch welche entfängt
    char buf = Serial2.read();
    Serial.print("BT_Received: ");
    Serial.println(buf);
    delay(5); // ------------------------------------------------- TODO: reduce this to 3 or maybe update up to 10??
    if (!m_isCmd && !m_isCardClicked) {
      switch (buf) {
        case '<': // Normales Command beginnt so
          Serial.println("Command Empfangen:");
          m_isCmd = true;
          break;
        case '!': // Wenn man in der App auf eine "Kachel" klickt
          Serial.println("Karte ausgewählt:");
          m_isCardClicked = true;
          break;
        case '%': // Wenn das BleutoothGerät verbunden ist
          m_isConnected = !m_isConnected;
          Serial.println("Verbunden!");
          return "";
          break;
      }
      continue;
    }

    // Command ist zu ende
    if (buf == '>')
      break;

    cmd += buf;     // add char to string
  }
  
  if (cmd != "")
	Serial.println(cmd); // Command wird im Seriellen Motitor ausgegeben

  return cmd;
}

/*
   Diese Methode wird aufgerufen, wenn das Bluetooth-Gerät das erste Mal (nach Programmstart) mit dem HC-05 Modul verbunden wird
*/
// Wenn das Gerät verbunden wurde...
void androidConnected()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Blueooth");
  lcd.setCursor(3, 1);
  lcd.print("Verbunden!");

  controller::warnblinkerAn();
  delay(500);
  controller::warnblinkerAus();
}

// Wenn die Verbindung unterbrochen wurde
void androidDisconnected()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BT Verbindung");
  lcd.setCursor(5, 1);
  lcd.print("getrennt!");
}

// wenn die Kachel Fernsteuerung ausgewählt wurde...
void fernsteuerung(String cmd)
{
  // Die Buchstaben (bytes) sagen, was gemacht werden soll... f für forwards, b für backwards, ...
  
  // Vorwärts und Rückwärts
  if (cmd == "f") // Vorwärts
  {
    controller::fahren(100);
  }
  else if (cmd == "b") // Rückwärts
  {
    controller::fahren(-100);
  }
  else if (cmd == "s") // stop
  {
    controller::anhalten();
  }

  ///////////////////////////////////////// Es sollte entewder die Abfrage oben oder unten abgefragt werden...

  // Vordere Achse lenken
  if (cmd == "r") // Rechts
  {
    controller::lenken(20);
  }
  else if (cmd == "l") // Links
  {
    controller::lenken(-20);
  }
  else if (cmd == "i")
  {
    controller::lenken(0);
  }
}

// Gibt Text auf LCD-Display aus
void lcdControll(String cmd)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(cmd);
}

// Wird nichtmehr gebraucht
void pinModes(String cmd)
{
  // Einschalten
  /*if (cmd == "llOn") // links
  {
    turnOnLed(blinkerLinks, 3);
  }
  else if (cmd  == "lrOn")
  {
    turnOnLed(blinkerRechts, 3);
  }

  // Ausschalten
  if (cmd == "llOff")
  {
    turnOffLed(blinkerLinks, 3);
  }
  else if (cmd == "lrOff")
  {
    turnOffLed(blinkerRechts, 3);
  }*/
}

// Warnbake wird angesteuert
void handleWarnbake(String cmd) {
  if (cmd == "aus") {
    warnbake::aus();
  } else if (cmd == "wand") {
    warnbake::wand();
  } else if (cmd == "pfeilL") {
    warnbake::pfeilLinks();
  } else if (cmd == "pfeilR") {
    warnbake::pfeilLinks();
  } else if (cmd == "blinkerB") {
    warnbake::blinkerBlau();
  } else if (cmd == "blinkerG") {
    warnbake::blinkerGelb();
  } else if (cmd == "MannG") {
    warnbake::mannGruen();
  } else if (cmd == "MannR") {
    warnbake::mannRot();
  } else if (cmd == "ampel") {
    warnbake::ampel();
  }
}

// Es wird geschaut welche Kachel in der App aktuell ausgewählt ist
void handleAppCard(String cmd) {
  switch (m_CardMode)
  {
    case NOTHING:
      Serial.println("NOTHING");
      // TODO
      break;
    case LCD_CONTROLL:
      lcdControll(cmd);
      break;
    case LED_SHIELD_NEW_ANIM:
      // 0x09 zum laden
      // TODO /// | (0xFF | LÄNGE | GERÜMPEL)
      break;
    case LED_SHIELD_STREAM_ANIM:
      // TODO
      break;
    case LED_SHIELD_UPLOAD_ANIM:
      handleWarnbake(cmd);
      // TODO
      break;
    case FERNSTEUERUNG:
      fernsteuerung(cmd);
      break;
    case SENSOREN:
      // TODO
      break;
    case TERMINAL:
      // TODO
      break;
    case PIN_MODE:
      pinModes(cmd);
      // TODO
      break;
    case MENU:
      // TODO
      break;
    case CARD_MODES_END:
      // TODO
      break;
  }
}

// Wenn man eine andere Kachel auswählt
const String m_sModes[] = {"nothing", "fernsteuerung", "lcd", "neostream", "neoupload", "neoanim", "pinmode", "sensor", "terminal", "menu"};
void changeMode(String mode) {
  for (int i = 0; i < CARD_MODES_END; i++)
  {
    if (mode == m_sModes[i])
    {
      //m_CardMode = FERNSTEUERUNG; /////////////////////////////////////////////////////////////// HIER IST DER FEHLER

      // Es wird geschaut we
      switch (i)
      {
        case 0:
          Serial.println("NOTHING");
          m_CardMode = NOTHING;
          break;
        case 1:
          m_CardMode = FERNSTEUERUNG;
          break;
        case 2:
          m_CardMode = LCD_CONTROLL;
          break;
        case 3:
          m_CardMode = LED_SHIELD_STREAM_ANIM;
          break;
        case 4:
          m_CardMode = LED_SHIELD_UPLOAD_ANIM;
          break;
        case 5:
          m_CardMode = LED_SHIELD_NEW_ANIM;
          break;
        case 6:
          m_CardMode = PIN_MODE;
          break;
        case 7:
          m_CardMode = SENSOREN;
          break;
        case 8:
          m_CardMode = TERMINAL;
          break;
        case 9:
          m_CardMode = MENU;
          break;
        case 10:
          m_CardMode = CARD_MODES_END;
          break;
        default:
          m_CardMode = NOTHING;
          break;
      }



      displayMode(m_sModes[i]);
      return;
    }
  }
  error("no mode");
}

void loop()
{
  // Wenn keine bytes übertragen werden... | oder kein Gerät verbunden ist
  /*if (Serial2.available() == 0 || (!m_isConnected && !m_wasAllreadyConnected))
    {
    delay(50);
    return;
    }*/

  // Liest aus, welche bytes übergeben werden und speichert sie in einem String
  String cmd = receiveCmd();

  // BluetoohGerät verbindet sich zum ersten mal mit dem HC-05 Modul:
  if (m_isConnected && !m_wasAllreadyConnected) {
    m_wasAllreadyConnected = true;
    androidConnected();
    return;
  }

  // BluetoohGerät hat die Verbindung getrennt
  if (!m_isConnected && m_wasAllreadyConnected) {
    // TODO: write a reset Method
    // reset();

    m_wasAllreadyConnected = false;
    androidDisconnected();
    return;
  }


  /******************************************************************* STATUS *************************************************/

  // Wenn eine Kachel angeklickt wurde bei der App
  if (m_isCardClicked)
  {
    m_isCardClicked = false;
    changeMode(cmd);
    return;
  }

  /******************************************************************* COMMAND *************************************************/

  // Auswertung des Commands, wenn eines übergeben wurde.
  if (m_isCmd)
  {
    Serial.print("---------- ICH BIN IN COMMANDS!!! ----------");
    m_isCmd = false;
    handleAppCard(cmd);
  }

  controller::loopController();
}
