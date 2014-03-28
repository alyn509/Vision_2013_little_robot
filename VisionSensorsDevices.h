#ifndef VisionSensorsDevices_h
#define VisionSensorsDevices_h

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
    boolean detectFront();
    boolean detectBack();
    boolean detectLeft();
    boolean detectRight();
    void ThrowNet();
    void startShooting();
    void stopShooting();
};

#endif
