#include <elapsedMillis.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include "VisionStepper.h"
#include "VisionSensorsDevices.h"
#include "pins_little_robot.h"
#include "little_robot_constants.h"

#define STATE_STOP -1
#define STATE_WAIT -2
#define STATE_WAIT_MOTORS_STOP -3
#define BLACK 100

elapsedMillis wait_time;
int time_to_wait, state_to_set_after_wait;
VisionStepper motorLeft;
VisionStepper motorRight;
sensors_and_devices SnD;

int state;
int shootedBalls = 0;

void setup()
{
  SnD.init();
  
  motorLeft.init();
  motorLeft.initPins(enablePinLeft, directionPinLeft, stepPinLeft);
  motorLeft.initDelays(startSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorLeft.initSizes(wheelDiameter, wheelRevolutionSteps);
  
  motorRight.init();
  motorRight.initPins(enablePinRight, directionPinRight, stepPinRight);
  motorRight.initDelays(startSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorRight.initSizes(wheelDiameter, wheelRevolutionSteps);
  
  pinMode(buttonTestPin, INPUT_PULLUP);
  delay(1000);
  state = 0;
}

void loop()
{  
  switch (state)
  {
    case 0:     //move forward
      MoveForward(50.0,4000);
      waitForMotorsStop(state + 2);
      break;
   case 1:
      Serial.println(SnD.detectColor());
      /*if(SnD.detectColor() == BLACK)
      {
        motorLeft.pause();
        motorRight.pause();
        state++;
      }*/
      break;
   case 2:                    //wait to complete and rotate left
      TurnRight(90);
      waitForMotorsStop(state + 1);
      break;
    case 3:                    //wait to complete and move forward   
      MoveForward(50.0,4000);
      waitForMotorsStop(0);
      break;
    case 4:                    //shoot balls 
        if(shootedBalls < 6)
        {
          if(shootedBalls == 0)
            SnD.startShooting();   
          MoveForward(4,2000);
          shootedBalls++;
          wait(1000, state);
        }
        else
        {     
          SnD.stopShooting();
          MoveForward(20,1500);
          waitForMotorsStop(state + 1);
        }
      break;
    case 5:        //wait to complete and rotate left
        ArcToLeft(100,5000,true);
          waitForMotorsStop(state + 1);
      break;
    case 6:             //wait to complete and move forward
        MoveForward(28.3,1500);
        waitForMotorsStop(state + 1);
      break;
    case 7:        //wait to complete and move backward
        MoveBackward(28.3,1500);
        waitForMotorsStop(state + 1);
      break;
    case 8:
        ArcToLeft(100,5000,false);
        waitForMotorsStop(state + 1);
      break;
    case 9:   
        MoveBackward(10,4000);;
        state = STATE_STOP;
      break;
    case 10:
      break;
    case STATE_STOP:   //stop and throw net
      SnD.ThrowNet();
      //stop
      break;
    case STATE_WAIT:
      if (wait_time > time_to_wait)
      {
        state = state_to_set_after_wait;
      }
      break;
    case STATE_WAIT_MOTORS_STOP:
      if (motorLeft.isOff() && motorRight.isOff())
      {
        state = state_to_set_after_wait;
      }
      break;
  }
  if (SnD.detectFront() || SnD.detectBack() || SnD.detectLeft() || SnD.detectRight())
  {
    motorLeft.pause();
    motorRight.pause();
  }
  else
  {
    motorLeft.unpause();
    motorRight.unpause();
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

void waitForMotorsStop(int state_after)
{
  state = STATE_WAIT_MOTORS_STOP;
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
  motorLeft.doDistanceInCm(distance);
  motorRight.doDistanceInCm(distance);
}

void TurnLeft(int angle)
{      
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();
  motorLeft.toggleDirection();        
  motorLeft.doSteps(wheelDiameter * 10 / 90 * angle);
  motorRight.doSteps(wheelDiameter * 10 / 90 * angle);
  /*
  motorLeft.doDistanceInCm(2 * PI * distanceBetweenWheels / 360 * angle);
  motorRight.doDistanceInCm(2 * PI * distanceBetweenWheels / 360 * angle);
  */  
}


void TurnRight(int angle)
{  
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.doSteps(wheelDiameter * 10 / 90 * angle);
  motorRight.doSteps(wheelDiameter * 10 / 90 * angle);
  /*
  motorLeft.doDistanceInCm(2 * PI * distanceBetweenWheels / 360 * angle);
  motorRight.doDistanceInCm(2 * PI * distanceBetweenWheels / 360 * angle);
  */
}

void ArcToLeft(int radius, int step_delay, boolean forward)
{
  motorLeft.setTargetDelay(step_delay * 2);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();
  motorLeft.doDistanceInCm(radius / 4);
  motorRight.doDistanceInCm(radius / 2);
}


void ArcToRight(int radius, int step_delay, boolean forward)
{
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay * 2);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();
  motorLeft.doDistanceInCm(radius / 2);
  motorRight.doDistanceInCm(radius / 4);
}
