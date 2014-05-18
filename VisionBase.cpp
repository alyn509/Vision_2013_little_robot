#include "VisionBase.h"

void VisionBase::init()
{
  
  frontLeft.initPin(frontLeftSensorPin);
  //frontFront.initPin(frontFrontSensorPin);
  frontRight.initPin(frontRightSensorPin);
  
  left.initPin(leftSensorPin);
  right.initPin(rightSensorPin);  
  back.initPin(backSensorPin);

  leftMotor.init();
  leftMotor.initDirectionForward(HIGH);
  leftMotor.initPins(leftMotorEnablePin, leftMotorDirectionPin, leftMotorStepPin);
  leftMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff);
  leftMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  rightMotor.init();
  rightMotor.initDirectionForward(LOW);
  rightMotor.initPins(rightMotorEnablePin, rightMotorDirectionPin, rightMotorStepPin);
  rightMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff);
  rightMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  directionMovement = NONE;
  obstructionDetected = false;
  pinMode(colorRedPin, INPUT);
  oppositeSide = (digitalRead(colorRedPin) == HIGH);// = false;
}

void VisionBase::setTacticDelays(int tactic)
{
  leftMotor.setTacticDelays(tactic);
  rightMotor.setTacticDelays(tactic);
}

void VisionBase::setStartDelays(unsigned long startDelay)
{
  leftMotor.initDelays(startDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff);
  rightMotor.initDelays(startDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff);
}

void VisionBase::moveForward(float distance, unsigned long step_delay)
{       
  directionMovement = FRONT;
  leftMotor.setTargetDelay(step_delay);         
  rightMotor.setTargetDelay(step_delay);
  if(!oppositeSide)
  {
    leftMotor.setDirectionForward();
    rightMotor.setDirectionForward();
  }
  else
  {
    leftMotor.setDirectionBackward();
    rightMotor.setDirectionBackward();
  }
  leftMotor.doDistanceInCm(distance);
  rightMotor.doDistanceInCm(distance);
}

void VisionBase::moveBackward(float distance, unsigned long step_delay)
{    
  directionMovement = BACK;
  leftMotor.setTargetDelay(step_delay);         
  rightMotor.setTargetDelay(step_delay);
  leftMotor.setDirectionBackward();
  rightMotor.setDirectionBackward();  
  if(!oppositeSide)
  {
    leftMotor.setDirectionBackward();
    rightMotor.setDirectionBackward();
  }
  else
  {    
    leftMotor.setDirectionForward();
    rightMotor.setDirectionForward();
  }
  leftMotor.doDistanceInCm(distance);
  rightMotor.doDistanceInCm(distance);
}

void VisionBase::turnLeft(int angle)
{
  directionMovement = LEFT;
  leftMotor.setTargetDelay(5000);         
  rightMotor.setTargetDelay(5000);
    leftMotor.setDirectionBackward();
    rightMotor.setDirectionForward();
  leftMotor.doRotationInAngle(angle);
  rightMotor.doRotationInAngle(angle); 
}

void VisionBase::turnRight(int angle)
{  
  directionMovement = RIGHT;
  leftMotor.setTargetDelay(5000);         
  rightMotor.setTargetDelay(5000);
    leftMotor.setDirectionForward();
    rightMotor.setDirectionBackward();
  leftMotor.doRotationInAngle(angle);
  rightMotor.doRotationInAngle(angle);
}

void VisionBase::setSpecial()
{
  leftMotor.setSpecial();
  rightMotor.setSpecial();
}

void VisionBase::resetSpecial()
{
  leftMotor.resetSpecial();
  rightMotor.resetSpecial();
}

void VisionBase::pause()
{
  leftMotor.pause();
  rightMotor.pause();
}

void VisionBase::unpause()
{
  leftMotor.unpause();
  rightMotor.unpause();
}

boolean VisionBase::frontDetected()
{
  return frontLeft.detect() || frontRight.detect();
}

/*
boolean VisionBase::leftDetected()
{
  return left.detect();
}

boolean VisionBase::rightDetected()
{
  return right.detect();
}
*/
boolean VisionBase::backDetected()
{
  return back.detect();
}

boolean VisionBase::isStopped()
{
  return leftMotor.isOff() && rightMotor.isOff();
}

boolean VisionBase::isPaused()
{
  return leftMotor.isPaused() && rightMotor.isPaused();
}

void VisionBase::checkObstructions()
{
  obstructionDetected = false;
  if ((frontDetected() && directionMovement == FRONT && !oppositeSide) || (frontDetected() && directionMovement == BACK && oppositeSide))
    obstructionDetected = true;
  /*if (leftDetected() && (directionMovement == LEFT || directionMovement == RIGHT))
    obstructionDetected = true;
  if (rightDetected() && (directionMovement == LEFT || directionMovement == RIGHT))
    obstructionDetected = true;*/
  if ((backDetected() && directionMovement == BACK && !oppositeSide) || (backDetected() && directionMovement == FRONT && oppositeSide))
    obstructionDetected = true;
}

void VisionBase::doLoop()
{
  leftMotor.doLoop();
  rightMotor.doLoop();
}

void VisionBase::stopNow()
{    
  leftMotor.stopNow();
  rightMotor.stopNow();
}

float VisionBase::getDistanceMadeSoFar()
{    
  return (leftMotor.getDistanceMadeSoFar() + rightMotor.getDistanceMadeSoFar()) / 2;
}
