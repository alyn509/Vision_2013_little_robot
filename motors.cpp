#include "motors.h"

#define STOPPED 0
#define STOPPING 1
#define STOPPING_ENABLE_ON 2
#define RUNNING 3
#define PAUSE 4
#define STARTING 5

void motors::init(int enablePin, int directionPin, int stepPin)
{
  this->enablePin = enablePin;
  this->directionPin = directionPin;
  this->stepPin = stepPin;
  
  wheelDiameter = 10.3; //cm
  wheelCircumference = wheelDiameter * PI;
  wheelRevolutionSteps = 200;
  stepCmRatio = wheelRevolutionSteps / wheelCircumference;
  
  directionPinState = HIGH;
  enablePinState = LOW;
  stepPinState = LOW;
  stepsMadeSoFar = 0;
  stepsRemaining = 0;
  maxSpeedDelay = 1500;
  startSpeedDelay = 4000;
  highPhaseDelay = 100;
  doSetup();
}

void motors::doSetup()
{
  pinMode(directionPin, OUTPUT);
  digitalWrite(directionPin, HIGH);
  
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, enablePinState);
  
  pinMode(stepPin, OUTPUT);
  digitalWrite(stepPin, stepPinState);
}

void motors::doLoop()
{
  switch (globalState) {
    case STOPPED:
      break;
    case STOPPING:
      enablePinState = LOW;
      digitalWrite(enablePin, enablePinState);
      globalState = STOPPED;
      break;
    case STOPPING_ENABLE_ON:
      if (stopTimer > 100)
        globalState = STOPPING;
      break;
    case RUNNING:
      if (((stepPinState == LOW) && (stepTimer > currentDelay)) ||
          ((stepPinState == HIGH) && (stepTimer > highPhaseDelay))
          )
      {
        stepTimer = 0;
        if (!foundTargetSpeed)
        {
          if (currentDelay > targetDelay)
          {
            stepSpeedCounter++;
            raiseSpeed = true;
          }
          else if (currentDelay < targetDelay)
          {
            stepSpeedCounter--;
            raiseSpeed = false;
          }
        }
        currentDelay = startSpeedDelay * 10 / sqrt(0.025 * stepSpeedCounter + 100);
        if (!foundTargetSpeed)
          if ((!raiseSpeed && currentDelay > targetDelay) ||
              (raiseSpeed && currentDelay < targetDelay))
              foundTargetSpeed = true;
        if (foundTargetSpeed)
          digitalWrite(13, HIGH);
        else
          digitalWrite(13, LOW);
        //Serial.println(currentDelay);
        stepsMadeSoFar++;
        stepsRemaining--;
        if (stepsRemaining <= stepSpeedCounter)
          setTargetDelay(startSpeedDelay);
        stepPinState = !stepPinState;
        digitalWrite(stepPin, stepPinState);
        if (stepsRemaining == 0)
        {
          globalState = STOPPING_ENABLE_ON;
          stopTimer = 0;
          break;
        }
        //Serial.println(currentStepDelay);
      }
      break;
    case PAUSE:
      break;
    case STARTING:
      enablePinState = HIGH;
      digitalWrite(enablePin, enablePinState);
      globalState = RUNNING;
      stepTimer = 0;
      stepSpeedCounter = 0;
      currentDelay = startSpeedDelay;
      foundTargetSpeed = false;
      break;
  }
}

void motors::pause()
{
  if (globalState == PAUSE)
    return;
  old_state = globalState;
  globalState = PAUSE;
}

void motors::unpause()
{
  globalState = old_state;
}

void motors::emergencyStop()
{
  /*
  if (stepsRemaining > numberOfDeaccelerationSteps)
    stepsRemaining = numberOfDeaccelerationSteps;
    */
}

void motors::setMaxSpeed()
{
  setTargetDelay(maxSpeedDelay);
}

void motors::setTargetDelay(int targetDelay)
{
  if (this->targetDelay == targetDelay)
    return;
  this->targetDelay = targetDelay;
  foundTargetSpeed = false;
}

boolean motors::isOff()
{
  return globalState == STOPPED;
}

void motors::setDirectionForward()
{
  directionPinState = HIGH;
  digitalWrite(directionPin, directionPinState);
}

void motors::toggleDirection()
{
  directionPinState = !directionPinState;
  digitalWrite(directionPin, directionPinState);
}


boolean motors::isAtTargetSpeed()
{
  return foundTargetSpeed;
}

void motors::doSteps(int stepNumber)
{
  stepsMadeSoFar = 0;
  stepsRemaining = stepNumber * 2; //a step is made out of a LOW to HIGH transition
  globalState = STARTING;
}


void motors::doDistanceInCm(float distance)
{
  doSteps(distance * stepCmRatio);
}
