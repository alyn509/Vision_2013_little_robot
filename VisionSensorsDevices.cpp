#include "VisionSensorsDevices.h"
#include "pins_little_robot.h"

#define STOPPED 0
#define STOPPING 1
#define RUNNING 2
#define STARTING 3

boolean FirstBallFlag = true;
 
boolean frontDetected = false;
boolean leftDetected = false;
boolean rightDetected = false;
boolean backDetected = false;
boolean blackLineDetected = false;
    
const int delayActions = 4000;
    
void sensors_and_devices::init()
{
  pinMode(1, INPUT);
  attachInterrupt(1, ColourSensor, CHANGE);
  
  pinMode(0, INPUT);
  attachInterrupt(FrontSenzorPin, SenzorFront, CHANGE);
  
  pinMode(4, INPUT);
  //attachInterrupt(4, SenzorLeft, CHANGE);
  
  pinMode(5, INPUT);
  //attachInterrupt(5, SenzorRight, CHANGE);
  
  pinMode(PrepareBallPin, OUTPUT);
  digitalWrite(PrepareBallPin, LOW);
  
  pinMode(ShootBallPin, OUTPUT);
  digitalWrite(ShootBallPin, LOW);
  
  pinMode(ThrowNetPin, OUTPUT);
  digitalWrite(ThrowNetPin, LOW);
}

void sensors_and_devices::startShooting()
{
  digitalWrite(PrepareBallPin, HIGH);
  digitalWrite(ShootBallPin, HIGH);
}

void sensors_and_devices::stopShooting()
{
    digitalWrite(PrepareBallPin, LOW);
    digitalWrite(ShootBallPin, LOW);
}

void sensors_and_devices::ThrowNet()
{
  digitalWrite(ThrowNetPin, HIGH);
  delay(delayActions);
  digitalWrite(ThrowNetPin, LOW);
}

void sensors_and_devices::SenzorFront()
{
  frontDetected = !frontDetected;
}

boolean sensors_and_devices::detectFront()
{
  return digitalRead(FrontSenzorPin);
}

void sensors_and_devices::SenzorBack()
{
  backDetected = !backDetected;
}

boolean sensors_and_devices::detectBack()
{
  return digitalRead(BackSenzorPin);
}

boolean sensors_and_devices::detectLeft()
{
  return digitalRead(LeftSenzorPin);
}

boolean sensors_and_devices::detectRight()
{
  return digitalRead(RightSenzorPin);
}

void sensors_and_devices::SenzorLeft()
{
  leftDetected = !leftDetected;
}

void sensors_and_devices::SenzorRight()
{
  rightDetected = !rightDetected;
}

void sensors_and_devices::ColourSensor()
{  
  attachInterrupt(BackSenzorPin, SenzorBack, CHANGE);
  blackLineDetected = !blackLineDetected;
}
