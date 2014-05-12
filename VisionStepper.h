#ifndef VisionStepper_h
#define VisionStepper_h

#include "Arduino.h"
#include <elapsedMillis.h>

class VisionStepper {
  public:
    void init();
    void initPins(int enablePin, int directionPin, int stepPin);
    void initDirectionForward(boolean forward);
    void initDelays(unsigned long startSpeedDelay, unsigned long highPhaseDelay, unsigned long maxSpeedDelay);
    void initSizes(float wheelDiameter, int wheelRevolutionSteps, float distanceBetweenWheels);
    void initStepCmRatio(float stepCmRatio);
    void doLoop();
    void toggleDirection();
    void setDirectionForward();
    void setDirectionBackward();
    void setTargetDelay(unsigned long targetDelay);
    boolean isOff();
    boolean isAtTargetSpeed();
    void doSteps(unsigned long stepNumber);
    void doDistanceInCm(float distance);
    void doRotationInAngle(float angle);
    void stopNow();
    void setMaxSpeed();
    void pause();
    void unpause();
    void setSpecial();
    void resetSpecial();
    void slowDown();
    void speedUp();

  private:
    void doSetup();
  private:
    int enablePin, directionPin, stepPin;
    int enablePinState, directionPinState, stepPinState;
    boolean forwardDirection;
    boolean special;
    int globalState;
    unsigned long stepsMadeSoFar, stepsRemaining;
    float maxSpeedDelay, startSpeedDelay, currentDelay, targetDelay, highPhaseDelay, pauseDelay;
    int stepSpeedCounter;
    int old_state;
    boolean foundTargetSpeed, raiseSpeed, pauseWhenFound;
    elapsedMicros stepTimer;
    elapsedMillis stopTimer, pauseTurnOff;
    
    float stepCmRatio; // steps for a cm
    float degreeStepRatio; //steps for a degree turn;
};

#endif

