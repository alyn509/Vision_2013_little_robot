#ifndef VisionBase_h
#define VisionBase_h

#include "Arduino.h"
#include "VisionStepper.h"
#include "VisionSensor.h"
#include "pins_little_robot.h"
#include "little_robot_constants.h"
#include <elapsedMillis.h>

#define NONE 0
#define FRONT 1
#define BACK 2
#define LEFT 3
#define RIGHT 4

class VisionBase {
  public:
    void init();
    void setStartDelays(int startDelay);
    void moveForward(float distance, int step_delay);
    void moveBackward(float distance, int step_delay);
    boolean frontDetected();
    boolean leftDetected();
    boolean rightDetected();
    boolean backDetected();
    void checkObstructions();
    void turnLeft(int angle);
    void turnRight(int angle);
    void setSpecial();
    void resetSpecial();
    void pause();
    void unpause();
    void doLoop();
    void stopNow();
    boolean isStopped();
  public:
    VisionStepper leftMotor, rightMotor;
    VisionSensor back, left, right, frontLeft, frontFront, frontRight;
    int directionMovement;
    boolean obstructionDetected, ignoreSensors;
};

#endif
