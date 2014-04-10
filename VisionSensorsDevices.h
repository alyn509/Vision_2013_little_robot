#ifndef VisionSensorsDevices_h
#define VisionSensorsDevices_h

#include "Arduino.h"
#include <elapsedMillis.h>

class sensors_and_devices {
  public:    
    void init();
    static void ColourSensor();
    boolean detectFront();
    boolean detectBack();
    boolean detectLeft();
    boolean detectRight();
    int detectColor();
    void ThrowNet();
    void shootBall();
    void startShooting();
    void startSpinningBallTray();
    void stopSpinningBallTray();
    void stopShooting();
};

#endif

