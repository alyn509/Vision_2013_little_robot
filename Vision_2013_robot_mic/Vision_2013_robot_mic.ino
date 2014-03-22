#include <elapsedMillis.h>

#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include "motors.h"
#include "sensors_and_devices.h"

#define DELAY_BETWEEN_TOGGLE 500
#define STATE_STOP 8
#define STATE_WAIT 9

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
void wait(int time_in_ms, int state);
void MoveForward(int, int);
void MoveBackward(int, int);
void TurnLeft(int, int);
void TurnRight(int, int);

void setup()
{
  
  motorLeft.init(enablePin, directionPin, stepPin);
  motorRight.init(enablePin2, directionPin2, stepPin2);
  pinMode(led, OUTPUT);
  pinMode(buttonTestPin, INPUT_PULLUP);
  digitalWrite(led, LOW);
  delay(1000);
  state = 0;
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
      if (motorLeft.isOff() && motorRight.isOff())
      {        
        TurnLeft(103,103);
        state++;
      }
      break;
    case 2:                    //wait to complete and move forward
      if (motorLeft.isOff() && motorRight.isOff())
      {        
        MoveForward(300,1500);
        state++;
      }
      break;
    case 3:                    //shoot balls 
      if (motorLeft.isOff() && motorRight.isOff())
      {             
        if(shootedBalls < 6)
        {
          if(shootedBalls == 0)
            SnD.startShooting();   
          MoveForward(40,10000);
          shootedBalls++;
          wait(2000, state);
        }
        else
        {     
          SnD.stopShooting();                    
          motorLeft.setMaxSpeed();        
          motorRight.setMaxSpeed();
          MoveForward(200,1500);
          state++;
        }
      }  
      break;
    case 4:        //wait to complete and rotate left
      if (motorLeft.isOff() && motorRight.isOff())
      {             
        TurnLeft(103,103);
        state++;
      }
      break;
    case 5:             //wait to complete and move forward
      if (motorLeft.isOff() && motorRight.isOff())
      {  
        MoveForward(200, 1500);
        state++;
      }
      break;
    case 6:        //wait to complete and move backward
      if (motorLeft.isOff() && motorRight.isOff())
      {        
        MoveBackward(300, 1500);
        state++;
      }
      break;
    case 7:
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