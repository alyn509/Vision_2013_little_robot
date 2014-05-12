#include "VisionBase.h"

void VisionBase::init()
{
  frontLeft.initPin(frontLeftSenzorPin);
  frontFront.initPin(frontFrontSenzorPin);
  frontRight.initPin(frontRightSenzorPin);
  
  left.initPin(leftSenzorPin);
  right.initPin(rightSenzorPin);  
  back.initPin(backSenzorPin);
  
  leftEncoder.initPin(leftMotorEncoderPin);
  leftMotor.init();
  leftMotor.initDirectionForward(HIGH);
  leftMotor.initPins(leftMotorEnablePin, leftMotorDirectionPin, leftMotorStepPin);
  leftMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  leftMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  rightEncoder.initPin(rightMotorEncoderPin);
  rightMotor.init();
  rightMotor.initDirectionForward(LOW);
  rightMotor.initPins(rightMotorEnablePin, rightMotorDirectionPin, rightMotorStepPin);
  rightMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  rightMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  directionMovement = NONE;
  obstructionDetected = false;
}

void VisionBase::setStartDelays(int startDelay)
{
  leftMotor.initDelays(startDelay, highPhaseDelay, maxSpeedDelay);
  rightMotor.initDelays(startDelay, highPhaseDelay, maxSpeedDelay); 
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
  leftMotor.setDirectionBackward();
  rightMotor.setDirectionForward();
  leftMotor.doRotationInAngle(angle);
  rightMotor.doRotationInAngle(angle); 
}

void VisionBase::turnRight(int angle)
{  
  directionMovement = RIGHT;
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
  return frontLeft.detect() && frontFront.detect() && frontRight.detect();
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
  leftEncoder.doLoop();
  leftMotor.doLoop();
  rightEncoder.doLoop();
  rightMotor.doLoop();
}

void VisionBase::stopNow()
{    
  leftMotor.stopNow();
  rightMotor.stopNow();
}
