#ifndef MENU_H
#define MENU_H

#include "fernsteuerung.h"
#include "hardware.h"

#ifdef CAR2
  #include "warnbake_mega.h"
#endif

namespace menu {
#ifdef CAR1
  typedef void(*Function)();

  struct MenuEntry {
    String name;
    Function onClick;
  };

  MenuEntry* curMenu; size_t curMenuSize;
  size_t curIndex;

  //Abkürzung
  MenuEntry ent(String name, Function onClick) { return MenuEntry{name, onClick}; }

  void fehlt() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SIA G1");
    lcd.setCursor(0,1);
    lcd.print("fehlt noch");
    delay(1500);
  }
  
  #define MOD_ENTER(mod) { lcd.clear(); mod::_setup(); while (mod::_loop()) { } }
#else
  void fehlt() {
  }
  
  #define MOD_ENTER(mod) {              mod::_setup(); while (mod::_loop()) { } }
#endif

  /////////////////////////////////zu implementieren/////////////////////////////////
  void _fernsteuerung()        { MOD_ENTER(fernsteuerung) }
  void _ellipse()              { fehlt(); }
  void _acht()                 { fehlt(); }
  void _wende()                { fehlt(); }
  void alleFahrfiguren()       { _ellipse(); _acht(); _wende(); }
  void _sicherheitstraining()  { fehlt(); }
  void _spazierenFahren()      { fehlt(); }
  void _einparken()            { fehlt(); }
  
  void _spurgefuehrt()         { fehlt(); }
  void _einachs()              { fehlt(); }
  void _zweiachs()             { fehlt(); }
  void _dackel()               { fehlt(); }
  void _torkel()               { fehlt(); }
  //////////////////////////////////////bis hier//////////////////////////////////////

#ifdef CAR1
  void enter();
  void enterSettings();
  void enterLenkung();
  void enterFahrfiguren();

  //Großgeschrieben: Link auf Menü (bis auf Fahrfiguren, weil es verwirren würde)
  MenuEntry entries[] = {
    ent("SETTINGS",         enterSettings),
    ent("Einparken",        _einparken),
    ent("Fahrfiguren",      enterFahrfiguren),
    ent("Fernsteuerung",    _fernsteuerung),
    ent("Parcours",         _sicherheitstraining),
    ent("Spazieren fahren", _spazierenFahren)
  };
  MenuEntry settings[] = {
    ent("BACK",      menu::enter),
    ent("LENKUNG",   enterLenkung)
  };
  MenuEntry lenkung[] = {
    ent("BACK",         enterSettings),
    ent("Einachs",      _einachs),
    ent("Zweiachs",     _zweiachs),
    ent("Spurgefuehrt", _spurgefuehrt),
    ent("Dackel",       _dackel),
    ent("Torkel",       _torkel)
  };
  MenuEntry fahrfiguren[] = {
    ent("BACK",    menu::enter),
    ent("alle",    alleFahrfiguren),
    ent("Ellipse", _ellipse),
    ent("Acht",    _acht),
    ent("Wende",   _wende)
  };

  void switchToEntry(const size_t index) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (curMenu == &menu::entries[0]) {
      lcd.print("MENU      SIA G1");
    } else if (curMenu == &settings[0]) {
      lcd.print("SETTINGS  SIA G1");
    } else if (curMenu == &lenkung[0]) {
      lcd.print("LENKUNG   SIA G1");
    } else if (curMenu == &fahrfiguren[0]) {
      lcd.print("FIGUREN   SIA G1");
    }
    lcd.setCursor(0,1);
    lcd.print(curMenu[index].name);
    curIndex = index;
  }

  template <size_t N>
  void switchToMenu(MenuEntry (&menu)[N]) {
    curMenu = menu;
    curMenuSize = N;
    switchToEntry(0);
  }

  void enter() {
    switchToMenu(menu::entries);
  }
  void enterSettings() {
    switchToMenu(settings);
  }
  void enterLenkung() {
    switchToMenu(lenkung);
  }
  void enterFahrfiguren() {
    switchToMenu(fahrfiguren);
  }

  void up() {
    if (curIndex == 0) {
      switchToEntry(curMenuSize - 1);
    } else {
      switchToEntry(curIndex - 1);
    }
  }

  void down() {
    const size_t nextIndex = curIndex + 1;
    if (nextIndex == curMenuSize) {
      switchToEntry(0);
    } else {
      switchToEntry(nextIndex);
    }
  }

  void ok() {
    lcd.clear();
    curMenu[curIndex].onClick();
    switchToEntry(curIndex);
  }
#endif

#ifdef CAR2
  void smartphoneSignal(byte signal) {
    switch (signal) {
      case 0:
        warnbake::aus();
        break;
      case 1:
        warnbake::wand();
        break;
      case 2:
        warnbake::pfeilLinks();
        break;
      case 3:
        warnbake::pfeilRechts();
        break;
      case 4:
        warnbake::blinkerBlau();
        break;
      case 5:
        warnbake::blinkerGelb();
        break;
      case 6:
        warnbake::mannGruen();
        break;
      case 7:
        warnbake::mannRot();
        break;
      case 9:
        warnbake::benutzerdefiniert();
        break;
      case 10:
        _einachs();
        break;
      case 11:
        _zweiachs();
        break;
      case 12:
        _spurgefuehrt();
        break;
      case 13:
        _dackel();
        break;
      case 14:
        _torkel();
        break;
      case 15:
        _einparken();
        break;
      case 16:
        _ellipse();
        break;
      case 17:
        _acht();
        break;
      case 18:
        _wende();
        break;
      case 19:
        _fernsteuerung();
        break;
      case 20:
        _sicherheitstraining();
        break;
      case 21:
        _spazierenFahren();
        break;
      case 255:
        warnbake::benutzerdefiniertNeu();
        break;
    }
  }
#endif
}

#endif //MENU_H
