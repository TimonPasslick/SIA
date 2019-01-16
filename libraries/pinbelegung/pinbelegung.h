#ifndef PINBELEGUNG_H
#define PINBELEGUNG_H

namespace pinbelegung {

  using Pin = int;

  constexpr Pin
    debugConsole    [ 2]{ 0,  1},
    lautsprecher    [ 2]{44, 45},
    motorregler     [ 2]{ 6,  7},
    schalter        [ 6]{38, 39, 40, 41, 42, 43},
    servos          [ 6]{ 5,  2,  3, 11, 12, 13},
    digitalIOs      [ 9]{16, 17, 19, 22, 23, 24, 25,  4,  8},
    analogeEingaenge[15]{ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14};

  constexpr Pin
    serielleEmpfaenger{18}, //RB14SCAN
    vBatSense{16};

  struct HBruecke {
    Pin wert, richtung;
  };
  constexpr HBruecke hBruecken[2]{
    {10, 14},
    { 9, 15}
  };

  struct LowSideOut {
    Pin ausgabe, rueckmeldung;
  };
  constexpr LowSideOut lowSide[5]{
    {32, 33},
    {34, 35},
    {36, 37},
    {46, 47},
    {48, 49}
  };

  namespace display {
    constexpr Pin
      clockEnable{30},
      registerSelect{31},
      datenBits4bis7[4]{29, 28, 27, 26};
  };

  namespace I2C {
    constexpr Pin
      SDA{20},
      SCL{21};
  }

  namespace SPI {
    constexpr Pin
      MISO{50},
      MOSI{51},
      SCK {52},
      CS  {53};
  }
} //namespace pinbelegung

#endif
