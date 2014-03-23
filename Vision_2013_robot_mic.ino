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

int enablePin = 7;
int stepPin = 6;
int directionPin = 5;

int buttonTestPin = 40;

int enablePin2 = 4;
int stepPin2 = 3;
int directionPin2 = 2;

int led = 13;

elapsedMillis wait_time;
int time_to_wait, state_to_set_after_wait;
motors motorLeft;
motors motorRight;
sensors_and_devices SnD;

int state;
int shootedBalls = 0;
boolean v1;
void wait(int time_in_ms, int state);
void MoveForward(int, int);
void MoveBackward(int, int);
void TurnLeft(int, int);
void TurnRight(int, int);

void setup()
{
  SnD.init();
  motorLeft.init(enablePin, directionPin, stepPin);
  motorRight.init(enablePin2, directionPin2, stepPin2);
  pinMode(led, OUTPUT);
  pinMode(buttonTestPin, INPUT_PULLUP);
  digitalWrite(led, LOW);
  delay(1000);
  state = 100;
  v1 = true;
  //Serial.begin(9600);
}

void loop()
{
  
  switch (state)
  {
    case 0:     //move forward
      MoveForward(300,1500);
      state++;
      break;
   case 1:                    //wait to complete and rotate left
      if (motorLeft.isOff() && motorRight.isOff() && !SnD.detectFront() && !SnD.detectBack())
      {        
        TurnLeft(400,400);
        //state++;
      }
      break;
    case 2:                    //wait to complete and move forward
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {        
        MoveForward(300,1500);
        if (v1)
          state++;
        else
          state = 15;
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
        }
      }  
      break;
    case 4:        //wait to complete and rotate left
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {             
        TurnLeft(103,103);
        state++;
      }
      break;
    case 5:             //wait to complete and move forward
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {  
        MoveForward(200, 1500);
        state++;
      }
      break;
    case 6:        //wait to complete and move backward
      if (motorLeft.isOff() && motorRight.isOff())// && !SnD.detectFront() && !SnD.detectBack())
      {        
        MoveBackward(300, 1500);
        state++;
      }
      break;
    case 7:
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
      motorLeft.setTargetDelay(1500);
      motorRight.setTargetDelay(1500);
      motorLeft.setDirectionForward();
      motorRight.setDirectionForward();
      motorRight.toggleDirection();     
      motorLeft.doDistanceInCm(10.3);
      motorRight.doDistanceInCm(10.3);
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

void MoveForward(int distance, int step_delay)
{       
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();     
  motorLeft.doSteps(distance);
  motorRight.doSteps(distance);
}

void MoveBackward(int distance, int step_delay)
{       
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.toggleDirection();     
  motorLeft.doSteps(distance);
  motorRight.doSteps(distance);
}

void TurnLeft(int leftMotor, int rightMotor)
{
  motorLeft.setTargetDelay(100);         
  motorRight.setTargetDelay(100);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();
  motorLeft.toggleDirection();        
  motorLeft.doSteps(leftMotor);
  motorRight.doSteps(rightMotor);
}


void TurnRight(int leftMotor, int rightMotor)
{  
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.doSteps(leftMotor);
  motorRight.doSteps(rightMotor);
}
