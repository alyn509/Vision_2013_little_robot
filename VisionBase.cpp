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
  leftMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay, pauseSpeedDelay);
  leftMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  rightMotor.init();
  rightMotor.initDirectionForward(LOW);
  rightMotor.initPins(rightMotorEnablePin, rightMotorDirectionPin, rightMotorStepPin);
  rightMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay, pauseSpeedDelay); 
  rightMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  directionMovement = NONE;
  obstructionDetected = false;
  oppositeSide /*= digitalRead(colorYellowPin)*/ = false;
}

void VisionBase::setStartDelays(unsigned long startDelay)
{
  leftMotor.initDelays(startDelay, highPhaseDelay, maxSpeedDelay, pauseSpeedDelay);
  rightMotor.initDelays(startDelay, highPhaseDelay, maxSpeedDelay, pauseSpeedDelay);
}

void VisionBase::moveForward(float distance, unsigned long step_delay)
{       
  directionMovement = FRONT;
  leftMotor.setTargetDelay(step_delay);         
  rightMotor.setTargetDelay(step_delay);
  leftMotor.setDirectionForward();
  rightMotor.setDirectionForward();
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
  leftMotor.doDistanceInCm(distance);
  rightMotor.doDistanceInCm(distance);
}

void VisionBase::turnLeft(int angle)
{
  directionMovement = LEFT;
  if(!oppositeSide)
  {
    leftMotor.setDirectionBackward();
    rightMotor.setDirectionForward();
  }
  else
  {    
    leftMotor.setDirectionForward();
    rightMotor.setDirectionBackward();
  }
  leftMotor.doRotationInAngle(angle);
  rightMotor.doRotationInAngle(angle); 
}

void VisionBase::turnRight(int angle)
{  
  directionMovement = RIGHT;
  if(!oppositeSide)
  {
    leftMotor.setDirectionForward();
    rightMotor.setDirectionBackward();
  }
  else
  { 
    leftMotor.setDirectionBackward();
    rightMotor.setDirectionForward();   
  }
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
  return frontLeft.detect() || /*frontFront.detect() &&*/ frontRight.detect();
}

boolean VisionBase::leftDetected()
{
  return left.detect();
}

boolean VisionBase::rightDetected()
{
  return right.detect();
}

boolean VisionBase::backDetected()
{
  return back.detect();
}

boolean VisionBase::isStopped()
{
  return leftMotor.isOff() && rightMotor.isOff();
}

void VisionBase::checkObstructions()
{
  obstructionDetected = false;
  if (frontDetected() && directionMovement == FRONT)
    obstructionDetected = true;
  if (leftDetected() && (directionMovement == LEFT || directionMovement == RIGHT))
    obstructionDetected = true;
  if (rightDetected() && (directionMovement == LEFT || directionMovement == RIGHT))
    obstructionDetected = true;
  if (backDetected() && directionMovement == BACK)
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
