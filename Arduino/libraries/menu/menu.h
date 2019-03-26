#ifndef MENU_H
#define MENU_H

//#include "..\sdk\sim\lib\arduino_sim\LiquidCrystal.h"
#include "LiquidCrystal.h"

const int rs = 31, en = 30, d4 = 29, d5 = 28, d6 = 27, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

namespace menu {
	
  typedef void(*Function)();

  struct MenuEntry {
    String name;
    Function onClick;
    
    MenuEntry(String Name, void (*OnClick)()) { name = Name; onClick = OnClick; }
  };

  MenuEntry* curMenu; size_t curMenuSize;
  size_t curIndex;

  //Abkürzung
  MenuEntry ent(String name, void (*onClick)()) { return MenuEntry(name, onClick); }

  void fehlt() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SIA G1");
    lcd.setCursor(0,1);
    lcd.print("fehlt noch");
    delay(1500);
  }

  /////////////////////////////////zu implementieren/////////////////////////////////
  void _fernsteuerung()        { fehlt(); }
  void _fahrfigur1()           { fehlt(); }
  void _fahrfigur2()           { fehlt(); }
  void _fahrfigur3()           { fehlt(); }
  void _sicherheitstraining()  { fehlt(); }
  void _spazierenFahren()      { fehlt(); }
  void _einparken()            { fehlt(); }
  
  void _spurgefuehrt()         { fehlt(); }
  void _einachs()              { fehlt(); }
  void _zweiachs()             { fehlt(); }
  void _dackel()               { fehlt(); }
  void _torkel()               { fehlt(); }
  //////////////////////////////////////bis hier//////////////////////////////////////
  
#define MOD_ENTER(mod) { mod::_setup(); while (mod::_loop()) { } }

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
    ent("BACK", menu::enter),
    ent("LENKUNG", enterLenkung)
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
    ent("Ellipse", _fahrfigur1),
    ent("Acht",    _fahrfigur2),
    ent("Wende",   _fahrfigur3)
  };

  void switchToEntry(const size_t index) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (curMenu == &menu::entries[0]) {
      lcd.print("SIA G1      MENU");
    } else if (curMenu == &settings[0]) {
      lcd.print("SIA G1  SETTINGS");
    } else if (curMenu == &lenkung[0]) {
      lcd.print("SIA G1   LENKUNG");
    } else if (curMenu == &fahrfiguren[0]) {
      lcd.print("SIA G1   FIGUREN");
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
}

#endif //MENU_H