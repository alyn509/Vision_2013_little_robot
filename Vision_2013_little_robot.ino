#include <elapsedMillis.h>
#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include "VisionStepper.h"
#include "VisionSensorsDevices.h"
#include "pins_little_robot.h"

#define DELAY_BETWEEN_TOGGLE 500
#define STATE_STOP -1
#define STATE_WAIT -2

int degree = 0;
boolean rotate = false;

elapsedMillis wait_time;
int time_to_wait, state_to_set_after_wait;
VisionStepper motorLeft;
VisionStepper motorRight;
sensors_and_devices SnD;

int state;
int shootedBalls = 0;
void wait(int time_in_ms, int state);
void MoveForward(float, int);
void MoveBackward(float, int);
void TurnLeft(int);
void TurnRight(int);
void ArcToRight(int, int, boolean);
void ArcToLeft(int, int, boolean);

void setup()
{
  SnD.init();
  
  motorLeft.init();
  motorLeft.initPins(enablePinLeft, directionPinLeft, stepPinLeft);
  motorLeft.initDelays(4000, 100, 1500); //start, high, max
  motorLeft.initSizes(10.3, 200); //diameter (cm), steps per revolution
  
  motorRight.init();
  motorRight.initPins(enablePinRight, directionPinRight, stepPinRight);
  motorRight.initDelays(4000, 100, 1500); //start, high, max
  motorRight.initSizes(10.3, 200); //diameter (cm), steps per revolution
  
  pinMode(buttonTestPin, INPUT_PULLUP);
  delay(1000);
  state = 0;
}

void loop()
{
  
  switch (state)
  {
    case 0:     //move forward
      MoveForward(283,4000);
      state++;
      degree=90;
      break;
   case 1:                    //wait to complete and rotate left
      if (motorLeft.isOff() && motorRight.isOff())
      {  
        ArcToLeft(300,1500,true);
        if(degree==0)
          state++;
      }
      break;
    case 2:                    //wait to complete and move forward
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {        
          MoveForward(283,1500);
          state++;
      }
      break;
    case 3:                    //shoot balls 
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {             
        if(shootedBalls < 6)
        {
          if(shootedBalls == 0)
            SnD.startShooting();   
          MoveForward(40,4000);
          shootedBalls++;
          wait(2000, state);
        }
        else
        {     
          SnD.stopShooting();
          MoveForward(200,1500);
          state++;
          degree=90;
        }
      }  
      break;
    case 4:        //wait to complete and rotate left
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {   
        ArcToLeft(300,1500,true);
        if(degree==0)
          state++;
      }
      break;
    case 5:             //wait to complete and move forward
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {         
        MoveForward(283,1500);
        state++;
      }
      break;
    case 6:        //wait to complete and move backward
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {        
        MoveBackward(283,1500);
        state++;
        degree=90;
      }
      break;
    case 7:
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {   
        ArcToLeft(300,1500,false);
        if(degree==0)
          state++;
      }
      break;
    case 8:
       if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {        
        MoveBackward(100,4000);;
        state++;
      }
      break;
    case 9:
         SnD.ThrowNet();
         //stop
      break;
    case STATE_STOP:   //stop and throw net
      //SnD.ThrowNet();
      //stop
      break;
    case STATE_WAIT:
      if (wait_time > time_to_wait)
      {
        state = state_to_set_after_wait;
      }
      break;
  }
  motorLeft.doLoop();
  motorRight.doLoop();
}

void wait(int time_in_ms, int state_after)
{
  state = STATE_WAIT;
  wait_time = 0;
  time_to_wait = time_in_ms;
  state_to_set_after_wait = state_after;
}

void MoveForward(float distance, int step_delay)
{       
  float counter = 0;
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();  
 /* while (counter < distance) 
  { 
    if(!SnD.detectFront())
    {
      motorLeft.doDistanceInCm(0.1);
      motorRight.doDistanceInCm(0.1);
      counter += 0.1;
    }
  }*/
  motorLeft.doDistanceInCm(distance);
  motorRight.doDistanceInCm(distance);
}

void MoveBackward(float distance, int step_delay)
{       
  float counter = 0;
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.toggleDirection();    
/*  while (counter < distance) 
  { 
    if(!SnD.detectBack())
    {
      motorLeft.doDistanceInCm(0.1);
      motorRight.doDistanceInCm(0.1);
      counter += 0.1;
    }
  }*/
  motorLeft.doDistanceInCm(distance);
  motorRight.doDistanceInCm(distance);
}

void TurnLeft(int angle)
{
  //motorLeft.setTargetDelay(100);         
  //motorRight.setTargetDelay(100);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();
  motorLeft.toggleDirection();        
  motorLeft.doDistanceInCm(103.0/angle * 10.3/200);
  motorRight.doDistanceInCm(103.0/angle * 10.3/200);
}


void TurnRight(int angle)
{  
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.doDistanceInCm(103.0/angle * 10.3/200);
  motorRight.doDistanceInCm(103.0/angle * 10.3/200);
}

void ArcToLeft(int radius, int step_delay, boolean forward)
{
  if(!rotate)
  {
    if(forward)
      MoveForward(radius * PI / 180, step_delay);
    else 
      MoveBackward(radius * PI / 180, step_delay);
    rotate = true;
  }
  else
  {
    TurnLeft(1);
    rotate = false;
    degree--;
  }
}


void ArcToRight(int radius, int step_delay, boolean forward)
{
  if(!rotate)
  {
    if(forward)
      MoveForward(radius * PI / 180, step_delay);
    else 
      MoveBackward(radius * PI / 180, step_delay);
    rotate = true;
  }
  else
  {
    TurnRight(1);
    rotate = false;
    degree--;
  }
}
