#ifndef EIGENE_PINS_H
#define EIGENE_PINS_H

#include "pinbelegung.h"

namespace eigenePins {

  using namespace pinbelegung;

  namespace sensor {
    namespace vorne {
      constexpr Pin
        gross {digitalIOs[0]},
        klein {digitalIOs[1]},
        links {digitalIOs[2]},
        rechts{digitalIOs[3]};
    }
    namespace hinten {
      constexpr Pin
        gross {digitalIOs[4]},
        klein {digitalIOs[5]},
        links {digitalIOs[6]},
        rechts{digitalIOs[7]};
    }
    constexpr Pin
      gyroskop{digitalIOs[8]},
      tageslicht{analogeEingaenge[0]};
  }

  namespace knopf {
    constexpr Pin
      hoch       {analogeEingaenge[1]},
      runter     {analogeEingaenge[2]},
      bestaetigen{analogeEingaenge[3]};
  }

  namespace blinker {
    constexpr LowSideOut
      rechts{lowSide[0]},
      links{lowSide[1]};
  }

  constexpr LowSideOut
    fahrtlicht    {lowSide[2]},
    rueckfahrlicht{lowSide[3]};

} //namespace eigenePins

#endif
