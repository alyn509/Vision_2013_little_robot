#ifndef VisionSensorsDevices_h
#define VisionSensorsDevices_h

#include "Arduino.h"
#include "VisionSensor.h"
#include <elapsedMillis.h>

class sensors_and_devices {
  public:    
    void init();
    VisionSensor back, left, right, front, front2;
    int shootingState;
    boolean pausedShooting;
    void ThrowNet();
    void shootBall();
    void startShooting();
    void startSpinningBallTray();
    void stopSpinningBallTray();
    boolean isSpinningBallTray();
    void pauseShooting();
    void resumeShooting();
    void stopShooting();
};

#endif

