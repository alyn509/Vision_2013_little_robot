#ifndef VisionSensorsDevices_h
#define VisionSensorsDevices_h

#include "Arduino.h"
#include <elapsedMillis.h>

class sensors_and_devices {
  public:    
    void init();
    static void ColourSensor();
    VisionSensor back, left, right, front;
    int detectColor();
    void ThrowNet();
    void shootBall();
    void startShooting();
    void startSpinningBallTray();
    void stopSpinningBallTray();
    void stopShooting();
};

#endif

