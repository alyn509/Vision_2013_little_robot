#include "sensors_and_devices.h"

#define STOPPED 0
#define STOPPING 1
#define RUNNING 2
#define STARTING 3

boolean FirstBallFlag = true;
const int ThrowNetPin = 15;
const int ColourPin1 = 0;   //  8 9 10 11 12 analog
const int ColourPin2 = 0;
const int ColourPin3 = 0;
<<<<<<< HEAD
const int ShootBallPin = 16;
const int PrepareBallPin = 17;
const int FrontSenzorPin = 18;
const int BackSenzorPin = 19;
const int LeftSenzorPin = 20;
const int RightSenzorPin = 21;
=======
const int ShootBallPin = 19;
const int PrepareBallPin = 20;
const int FrontSenzorPin = 17;
const int BackSenzorPin = 18;
const int LeftSenzorPin = 43;
const int RightSenzorPin = 42;
>>>>>>> origin/tests
 
boolean frontDetected = false;
boolean leftDetected = false;
boolean rightDetected = false;
boolean backDetected = false;
boolean blackLineDetected = false;
    
const int StopLedsPin = 14; // maybe not
const int LeftSignalingLedsPin = 16; // maybe not
const int RightSignalingLedsPin = 17; // maybe not
const int delayActions = 4000;
    
void sensors_and_devices::init()
{
  pinMode(1, INPUT);
  attachInterrupt(1, ColourSensor, CHANGE);
  
  pinMode(0, INPUT);
  attachInterrupt(FrontSenzorPin, SenzorFront, CHANGE);
  
  pinMode(4, INPUT);
  attachInterrupt(4, SenzorLeft, CHANGE);
  
  pinMode(5, INPUT);
  attachInterrupt(5, SenzorRight, CHANGE);
  
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
