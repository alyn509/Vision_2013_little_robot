#include <elapsedMillis.h>

#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include "motors.h"
#include "sensors_and_devices.h"

#define DELAY_BETWEEN_TOGGLE 500
#define STATE_STOP -1
#define STATE_WAIT -2

int enablePin = 8;
int stepPin = 9;
int directionPin = 10;

int buttonTestPin = 40;

int enablePin2 = 11;
int stepPin2 = 12;
int directionPin2 = 13;

int led = 13;
int degree = 0;
boolean rotate = false;

elapsedMillis wait_time;
int time_to_wait, state_to_set_after_wait;
motors motorLeft;
motors motorRight;
sensors_and_devices SnD;

int state;
int shootedBalls = 0;
boolean v1;
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
  motorLeft.init(enablePin, directionPin, stepPin);
  motorRight.init(enablePin2, directionPin2, stepPin2);
  pinMode(led, OUTPUT);
  pinMode(buttonTestPin, INPUT_PULLUP);
  digitalWrite(led, LOW);
  delay(1000);
  state = 0;
  v1 = true;
  //Serial.begin(9600);
}

void loop()
{
  
  switch (state)
  {
    case 0:     //move forward
<<<<<<< HEAD
      MoveForward(283,4000);
=======
      MoveForward(300,1500);
>>>>>>> origin/tests
      state++;
      degree=90;
      break;
   case 1:                    //wait to complete and rotate left
      if (motorLeft.isOff() && motorRight.isOff())
<<<<<<< HEAD
      {  
        ArcToLeft(300,1500,true);
        if(degree==0)
          state++;
=======
      {        
        TurnLeft(400,400);
        state++;
>>>>>>> origin/tests
      }
      break;
    case 2:                    //wait to complete and move forward
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {        
<<<<<<< HEAD
          MoveForward(283,1500);
          state++;
=======
        MoveForward(300,1500);
        if (v1)
          state++;
        else
          state = 15;
>>>>>>> origin/tests
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
<<<<<<< HEAD
      {   
        ArcToLeft(300,1500,true);
        if(degree==0)
          state++;
=======
      {             
        TurnLeft(103,103);
        SnD.ThrowNet();
        state++;
>>>>>>> origin/tests
      }
      break;
    case 5:             //wait to complete and move forward
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
<<<<<<< HEAD
      {         
        MoveForward(283,1500);
=======
      {  
        MoveForward(200, 1500);
>>>>>>> origin/tests
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
    case 15:
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {             
        SnD.startShooting();   
        MoveForward(240,4000);
        state++;
      }
      break;
    case 16:
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {
        SnD.stopShooting();
        MoveForward(200,1500);
        state = 4;
      }
      break;
    case 100:
      motorLeft.setTargetDelay(3000);
      motorRight.setTargetDelay(3000);
      motorLeft.setDirectionForward();
      motorRight.setDirectionForward();
      motorRight.toggleDirection();     
      motorLeft.doDistanceInCm(20);
      motorRight.doDistanceInCm(20);
      state = STATE_STOP;
      break;
    case 101:
      motorLeft.setTargetDelay(300);
      motorRight.setTargetDelay(300);
      motorLeft.setDirectionForward();
      motorRight.setDirectionForward();
      motorRight.toggleDirection();     
      motorLeft.doDistanceInCm(5);
      motorRight.doDistanceInCm(5);
      state = STATE_STOP;
      break;
    case 102:
      motorLeft.setTargetDelay(300);
      motorRight.setTargetDelay(300);
      motorLeft.setDirectionForward();
      motorRight.setDirectionForward();
      motorRight.toggleDirection();     
      motorLeft.doDistanceInCm(20);
      motorRight.doDistanceInCm(20);
      state = STATE_STOP;
      break;
    case 200:
      Serial.print(SnD.detectFront());
      Serial.print(" ");
      Serial.println(SnD.detectBack());
      break;
    case 300:
      MoveForward(30000, 200);
      state = 301;
      break;
    case 301:
      if (motorLeft.isOff() && motorRight.isOff())
        state = STATE_STOP;
      if (SnD.detectFront() || SnD.detectBack())
      {
        motorLeft.pause();
        motorRight.pause();
      }
      else
      {
        motorLeft.unpause();
        motorRight.unpause();
      }
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
<<<<<<< HEAD
  //motorLeft.setTargetDelay(100);         
  //motorRight.setTargetDelay(100);
=======
  motorLeft.setTargetDelay(100);         
  motorRight.setTargetDelay(100);
>>>>>>> origin/tests
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
