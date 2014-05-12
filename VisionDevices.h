#ifndef VisionSensorsDevices_h
#define VisionSensorsDevices_h

#include "Arduino.h"
#include "VisionSensor.h"
#include <elapsedMillis.h>

class VisionDevices {
  public:    
    void init();
    void ThrowNet();
    void shootBall();
    void startShooting();
    void startSpinningBallTray();
    void stopSpinningBallTray();
    void stopShooting();
};

#endif

