#ifndef little_robot_constants_h
#define little_robot_constants_h

const unsigned long defaultStartSpeedDelay = 50000L;
const unsigned long pauseSpeedDelay = 50000L;
const unsigned long highPhaseDelay = 100;
const float stepSpeedCounterAcceleration = 40;
const float stepSpeedCounterSlowing = 20;
const unsigned long delayBeforeTurnOff = 500;

const unsigned long slowSpeedDelay = 7000;
const unsigned long fastSpeedDelay = 1000;
const unsigned long mediumSpeedDelay = 3000;
const unsigned long ultraSlowSpeedDelay = 100000L;

const float wheelDiameter = 10.5;
const float distanceBetweenWheels = 15.7; //valoare interioara
const int wheelRevolutionSteps = 200;

const float encoderRevolutionSteps = 24;

#endif
