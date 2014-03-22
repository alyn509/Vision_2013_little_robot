#ifndef sensors_and_devices_h
#define sensors_and_devices_h

#include "Arduino.h"
#include <elapsedMillis.h>

class sensors_and_devices {
  public:    
    void init();
    static void SenzorFront();
    static void SenzorLeft();
    static void SenzorRight();
    static void SenzorBack();
    static void ColourSensor();
    void ThrowNet();
    void startShooting();
    void stopShooting();
};

#endif
