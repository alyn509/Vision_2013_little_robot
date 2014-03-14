#include <Stepper.h>
#include <TimerThree.h>

#include "motors.h"
/*
  1.8 degrees / step
  200 steps -> 360 degrees
  Encoders + Senzors - 2, 3, 21, 20, 19, 18 
  colours : 1-red, 2-blue, 3-green
*/
boolean FirstBallFlag = true;

const int ThrowNetPin = 7;
const int ShootBallPin = 9;
const int BallHolderPin = 33;
const int PrepareBallPin = 32;

const int ColourPin = 8; // 0 - left/red , 1 - right/yellow
const int StrategyPin = 49; // in case of more strategies

const int FrontSenzorPin = 46;
const int BackSenzorPin = 45;
const int LeftSenzorPin = 43;
const int RightSenzorPin = 42;

const int StopLedsPin = 14; // maybe not
const int LeftSignalingLedsPin = 16; // maybe not
const int RightSignalingLedsPin = 17; // maybe not

int selectedSide = 0;
int selectedStrategy = 0; // in case of more strategies
// int teamColour = 2;
int strategyStep = 0;

const float fastSpeedDelay = 3;
const float slowSpeedDelay = 5;
const float accelerationSteps = 80;
const float delayRate = 0.025;

// 0 - left , 1 - right
int motorEnablePins[] = {51,52};

byte motorSteps[4][4] = {
                          {B10101010, B10100110, B10100101, B10101001},
                          {B01101010, B01100110, B01100101, B01101001},
                          {B01011010, B01010110, B01010101, B01011001},
                          {B10011010, B10010110, B10010101, B10011001}
                          };

int motorCounters[2] = {0,0}; //  index: 0 - left, 1 - right
const int wheelDiameter = 10.3; // cm   
const int wheelRevolutionSteps = 200;
const float stepCmRatio = (float)wheelRevolutionSteps/wheelDiameter; // steps for a cm
const float degreeStepRatio = 5.8; // steps for a degree
String direction = "";
volatile boolean frontDetected = false;
volatile boolean leftDetected = false;
volatile boolean rightDetected = false;
volatile boolean backDetected = false;
volatile int stepsLeft = 0;
volatile int stepsRight = 0;
boolean completeSteps = false;
const int delayActions = 1500;
      
void MotionMotors();
void MoveForward(float);
void MoveBackwards(float);
void TurnLeft(int);
void TurnRight(int);
void EncoderLeft();
void EncoderRight();
void SenzorFront();
void SenzorLeft();
void SenzorRight();
void SenzorBack();
void ThrowNet();
void ShootBall();
void StopMotors();
void StartMotors();
void StopEverything();
        
void setup()
{
  Timer3.initialize();
  Timer3.attachInterrupt(StopEverything,1000000);
  pinMode(47, OUTPUT);
  pinMode(47, HIGH);
  pinMode(StopLedsPin, OUTPUT);
  pinMode(LeftSignalingLedsPin, OUTPUT);
  pinMode(RightSignalingLedsPin, OUTPUT);
  pinMode(ShootBallPin, OUTPUT);
//  TCS3200setup();
  
//  attachInterrupt(0, EncoderLeft, FALLING);
//  attachInterrupt(1, EncoderRight, FALLING);
  attachInterrupt(1, SenzorBack, CHANGE);
  attachInterrupt(0, SenzorFront, CHANGE);
  attachInterrupt(4, SenzorLeft, CHANGE);
  attachInterrupt(5, SenzorRight, CHANGE);
  
  StartMotors();
    
  DDRA = B11111111; // set PORTA pins to output -> 22-29
  if(analogRead(ColourPin)<=800)
  {
    teamColour = 2;
    selectedSide = 0;
  }else
  {
    teamColour = 1;
    selectedSide = 1;
  }
  if(digitalRead(StrategyPin) == LOW)
  {
    selectedStrategy = 0;
  }else
  {
    selectedStrategy = 1;
  }
  strategyStep = 0;
}

void loop()
{

switch(selectedSide)
{
  case 0: //left/red
        {
          switch(selectedStrategy)
          {
            case 0:
                  {
                    switch(strategyStep)
                    {
                       case 0:
                            {
                              delay(10000);
                              MoveForward(0);
                              TurnRight();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();                             
                              
                              strategyStep++;
                              break;
                            }
                       case 1:
                            {
                              ThrowNet();  
                              strategyStep++;
                              break;
                            }     
                       case 2:
                            {                            
                              MoveForward(0);
                              TurnRight();
                              MoveForward(0);
                              MoveBackward(0);
                              strategyStep++;
                              break;
                            }
                       case 3:
                            {
                              StopMotors();
                              strategyStep++;
                              break;
                            }     
                    }
                    break;
                  }
            case 1: 
                  {
                    switch(strategyStep)
                    {
                      case 0:
                            {
                            StopMotors();
                            strategyStep++;
                            break;
                            }
                    }
                    break;
                  }
          }
        break;  
        }
  case 1: //right/yellow
        {
           switch(selectedStrategy)
          {
            case 0:
                  {
                    switch(strategyStep)
                    {
                       case 0:
                            {
                              delay(10000);
                              MoveForward(0);
                              TurnLeft();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();
                              MoveForward(0);
                              ShootBall();                             
                              
                              strategyStep++;
                              break;
                            }
                       case 1:
                            {
                              ThrowNet();  
                              strategyStep++;
                              break;
                            }     
                       case 2:
                            {                            
                              MoveForward(0);
                              TurnLeft();
                              MoveForward(0);
                              MoveBackward(0);
                              strategyStep++;
                              break;
                            }
                       case 3:
                            {
                              StopMotors();
                              strategyStep++;
                              break;
                            }     
                    }
                    break;
                  }
            case 1:
                  {
                    switch(strategyStep)
                    {
                      case 0:
                            {
                            StopMotors();
                            strategyStep++;
                            break;
                            }
                    }
                    break;
                  }
          } 
          break;
        } 
}

}

void StartMotors()
{
  for(int counter = 0;counter<3;counter++)
  {
    pinMode(motorEnablePins[counter], OUTPUT);
    digitalWrite(motorEnablePins[counter], HIGH);
  }
}

void StopMotors()
{
  for(int counter = 0;counter<3;counter++)
  {
    digitalWrite(motorEnablePins[counter], LOW);
  }
}

void StopEverything()
{
    StopMotors();
}

void ThrowNet()
{
  digitalWrite(ThrowNetPin, HIGH);
  delay(delayActions);
  digitalWrite(ThrowNetPin, LOW);
}

void ShootBall()
{
  if(FirstBallFlag == true)
  {
    digitalWrite(ShootBallPin, HIGH);
    FirstBallFlag = false;
    digitalWrite(BallHolderPin, HIGH);
    delay(500); 
    digitalWrite(BallHolderPin, LOW);
  }
  else
  {
    digitalWrite(PrepareBallPin, HIGH);
    delay(1500);
    digitalWrite(PrepareBallPin, LOW);
  }
}

void SenzorFront()
{
  frontDetected = !frontDetected;
}

void SenzorBack()
{
  backDetected = !backDetected;
}

void SenzorLeft()
{
  leftDetected = !leftDetected;
}

void SenzorRight()
{
  rightDetected = !rightDetected;
}

void EncoderLeft()
{
  stepsLeft++;
}

void EncoderRight()
{
  stepsRight++;
}

void MotionMotors()
{
  PORTA = motorSteps[motorCounters[0]][motorCounters[1]];
}

void MoveForward(float distance)
{
  int steps = distance * stepCmRatio;
  stepsLeft = 0;
  stepsRight = 0;
  float stepDelay = slowSpeedDelay;
  int deccelerationSteps = steps - accelerationSteps;
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
  while(digitalRead(FrontSenzorPin) == HIGH){
    digitalWrite(RightSignalingLedsPin, HIGH);
    digitalWrite(LeftSignalingLedsPin, HIGH);
    digitalWrite(StopLedsPin, HIGH);
    delay(250);
    digitalWrite(RightSignalingLedsPin, LOW);
    digitalWrite(LeftSignalingLedsPin, LOW);
    digitalWrite(StopLedsPin, LOW);
    delay(250);
  }
    MotionMotors();
    
    motorCounters[0]++;
    if(motorCounters[0] == 4){
      motorCounters[0] = 0;
    }
    
    motorCounters[1]++;
    if(motorCounters[1] == 4){
      motorCounters[1] = 0;
    }
    
    if(stepCounter<accelerationSteps)
    {
      stepDelay -= delayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += delayRate;
      digitalWrite(StopLedsPin,HIGH);
    }
    delay(stepDelay);
  }
  digitalWrite(StopLedsPin,LOW);
//  while(stepsLeft!=steps)
//  {
//    MotionMotors();
//    motorCounters[0]++;
//    if(motorCounters[0] == 4){
//      motorCounters[0] = 0;
//    }
//    delay(slowSpeedDelay);
//  }
//  while(stepsRight!=steps)
//  {
//    MotionMotors();
//    motorCounters[1]++;
//    if(motorCounters[1] == 4){
//      motorCounters[1] = 0;
//    }
//    delay(slowSpeedDelay);
//  }
}

void MoveBackwards(float distance)
{
  int steps = distance * stepCmRatio;
  stepsLeft = 0;
  stepsRight = 0;
  float stepDelay = slowSpeedDelay;
  int deccelerationSteps = steps - accelerationSteps;
  
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
    while(digitalRead(BackSenzorPin) == HIGH)
    {
      digitalWrite(RightSignalingLedsPin, HIGH);
      digitalWrite(LeftSignalingLedsPin, HIGH);
      digitalWrite(StopLedsPin, HIGH);
      delay(250);
      digitalWrite(RightSignalingLedsPin, LOW);
      digitalWrite(LeftSignalingLedsPin, LOW);
      digitalWrite(StopLedsPin, LOW);
      delay(250);
    }
    MotionMotors();
    
    motorCounters[0]--;
    if(motorCounters[0] == -1)
    {
      motorCounters[0] = 3;
    }
    
    motorCounters[1]--;
    if(motorCounters[1] == -1)
    {
      motorCounters[1] = 3;
    }
    if(stepCounter<accelerationSteps)
    {
      stepDelay -= delayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += delayRate;
      digitalWrite(StopLedsPin,HIGH);
    }
    delay(stepDelay);
  }
  digitalWrite(StopLedsPin,LOW);
//  while(stepsLeft!=steps)
//  {
//    MotionMotors();
//    motorCounters[0]--;
//    if(motorCounters[0] == -1)
//    {
//      motorCounters[0] = 3;
//    }
//  }
//  while(stepsRight!=steps)
//  {
//    MotionMotors();
//    motorCounters[1]--;
//    if(motorCounters[1] == -1)
//    {
//      motorCounters[1] = 3;
//    }
//  }
}

void TurnLeft(int degreeCount)
{
  int steps = degreeCount * degreeStepRatio;
  float stepDelay = slowSpeedDelay;
    int deccelerationSteps = steps - accelerationSteps;
    digitalWrite(LeftSignalingLedsPin, HIGH);
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
    while(digitalRead(LeftSenzorPin) == HIGH)
    {
      digitalWrite(RightSignalingLedsPin, HIGH);
      digitalWrite(LeftSignalingLedsPin, HIGH);
      digitalWrite(StopLedsPin, HIGH);
      delay(250);
      digitalWrite(RightSignalingLedsPin, LOW);
      digitalWrite(LeftSignalingLedsPin, LOW);
      digitalWrite(StopLedsPin, LOW);
      delay(250);
    }
    MotionMotors();
    
    motorCounters[0]--;
    if(motorCounters[0] == -1)
    {
      motorCounters[0] = 3;
    }
    motorCounters[1]++;
    if(motorCounters[1] == 4){
      motorCounters[1] = 0;
    }
    if(stepCounter<accelerationSteps)
    {
      stepDelay -= delayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += delayRate;
      digitalWrite(StopLedsPin,HIGH);
    }
    delay(stepDelay);
  }
  digitalWrite(LeftSignalingLedsPin, LOW);
  digitalWrite(StopLedsPin,LOW);
}

void TurnRight(int degreeCount)
{
  int steps = degreeCount * degreeStepRatio;
  float stepDelay = slowSpeedDelay;
    int deccelerationSteps = steps - accelerationSteps;
    digitalWrite(RightSignalingLedsPin, HIGH);
    
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
    while(digitalRead(RightSenzorPin) == HIGH)
    {
      digitalWrite(RightSignalingLedsPin, HIGH);
      digitalWrite(LeftSignalingLedsPin, HIGH);
      digitalWrite(StopLedsPin, HIGH);
      delay(250);
      digitalWrite(RightSignalingLedsPin, LOW);
      digitalWrite(LeftSignalingLedsPin, LOW);
      digitalWrite(StopLedsPin, LOW);
      delay(250);
    }
    MotionMotors();
    
    motorCounters[0]++;
    if(motorCounters[0] == 4){
      motorCounters[0] = 0;
    }
    motorCounters[1]--;
    if(motorCounters[1] == -1)
    {
      motorCounters[1] = 3;
    }
    if(stepCounter<accelerationSteps)
    {
      stepDelay -= delayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += delayRate;
      digitalWrite(StopLedsPin,HIGH);
    }
    delay(stepDelay);
  }
  digitalWrite(StopLedsPin,LOW);
  digitalWrite(RightSignalingLedsPin, LOW);
}

