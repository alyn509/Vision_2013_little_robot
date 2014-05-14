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
  leftMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  leftMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  rightMotor.init();
  rightMotor.initDirectionForward(LOW);
  rightMotor.initPins(rightMotorEnablePin, rightMotorDirectionPin, rightMotorStepPin);
  rightMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  rightMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  directionMovement = NONE;
  obstructionDetected = false;
  oppositeSide = false;
}

void VisionBase::doLoop()
{
  leftMotor.doLoop();
  rightMotor.doLoop();
}

void VisionBase::moveForward(float distance, int step_delay)
{
  directionMovement = FRONT;
  leftMotor.setTargetDelay(step_delay);         
  rightMotor.setTargetDelay(step_delay);
  leftMotor.setDirectionForward();
  rightMotor.setDirectionForward();
  leftMotor.doDistanceInCm(distance);
  rightMotor.doDistanceInCm(distance);
}

void VisionBase::moveBackward(float distance, int step_delay)
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
  leftMotor.setTargetDelay(2000);         
  rightMotor.setTargetDelay(2000);
  if(oppositeSide)
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
  leftMotor.setTargetDelay(2000);         
  rightMotor.setTargetDelay(2000);
  if(oppositeSide)
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

void VisionBase::stopNow()
{
  leftMotor.stopNow();
  rightMotor.stopNow();
}

boolean VisionBase::frontDetected()
{
  return frontLeft.detect() || /*frontFront.detect() ||*/ frontRight.detect();
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

/********************************************************* Old arcs *****************************************************/

void VisionBase::ArcToLeft(int radius, int step_delay, boolean forward)
{
  leftMotor.setTargetDelay(step_delay * 2);         
  rightMotor.setTargetDelay(step_delay);
  leftMotor.setDirectionForward();
  rightMotor.setDirectionForward();
  if(forward) 
    rightMotor.toggleDirection();
  else
    leftMotor.toggleDirection();
  leftMotor.doDistanceInCm(radius / 4);
  rightMotor.doDistanceInCm(radius / 2);
  
  /* leftMotor.setTargetDelay(8000);         
  rightMotor.setTargetDelay(1000);
  leftMotor.setDirectionForward();
  rightMotor.setDirectionForward();
  leftMotor.doDistanceInCm(300);
  rightMotor.doDistanceInCm(300);*/
}

void VisionBase::ArcToRight(int radius, int step_delay, boolean forward)
{
  leftMotor.setTargetDelay(step_delay);         
  rightMotor.setTargetDelay(step_delay * 2);
  leftMotor.setDirectionForward();
  rightMotor.setDirectionForward();
  if(forward) 
    rightMotor.toggleDirection();
  else
    leftMotor.toggleDirection();
  leftMotor.doDistanceInCm(radius / 2);
  rightMotor.doDistanceInCm(radius / 4);
}

