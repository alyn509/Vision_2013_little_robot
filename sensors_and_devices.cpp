#include "sensors_and_devices.h"

#define STOPPED 0
#define STOPPING 1
#define RUNNING 2
#define STARTING 3

boolean FirstBallFlag = true;
const int ThrowNetPin = 21;
const int ColourPin1 = 0;
const int ColourPin2 = 0;
const int ColourPin3 = 0;
const int ShootBallPin = 19;
const int PrepareBallPin = 20;
const int FrontSenzorPin = 46;
const int BackSenzorPin = 45;
const int LeftSenzorPin = 43;
const int RightSenzorPin = 42;
 
boolean frontDetected = false;
boolean leftDetected = false;
boolean rightDetected = false;
boolean backDetected = false;
boolean blackLineDetected = false;
    
const int StopLedsPin = 14; // maybe not
const int LeftSignalingLedsPin = 16; // maybe not
const int RightSignalingLedsPin = 17; // maybe not
const int delayActions = 1500;
    
void sensors_and_devices::init()
{
  pinMode(1, INPUT);
  attachInterrupt(1, ColourSensor, CHANGE);
  
  pinMode(0, INPUT);
  attachInterrupt(0, SenzorFront, CHANGE);
  
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

void sensors_and_devices::SenzorBack()
{
  backDetected = !backDetected;
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
  attachInterrupt(1, SenzorBack, CHANGE);
  blackLineDetected = !blackLineDetected;
}
