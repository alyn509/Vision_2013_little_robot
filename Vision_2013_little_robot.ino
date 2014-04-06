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
int shotBalls = 0;

void setup()
{
  SnD.init();
 // Serial.begin(9600);
  
  motorLeft.init();
  motorLeft.initPins(enablePinLeft, directionPinLeft, stepPinLeft);
  motorLeft.initDelays(startSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorLeft.initSizes(wheelDiameter, wheelRevolutionSteps);
  
  motorRight.init();
  motorRight.initPins(enablePinRight, directionPinRight, stepPinRight);
  motorRight.initDelays(startSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorRight.initSizes(wheelDiameter, wheelRevolutionSteps);
  
  //pinMode(buttonTestPin, INPUT_PULLUP);
  delay(1000);
  state = 0;
}

void loop()
{  
  switch (state)
  {
    case 0:     //move forward
      MoveForward(60,slowSpeedDelay);
      waitForMotorsStop(state + 2);
      break;
   case 1:
      //Serial.println(SnD.detectColor());
      /*if(SnD.detectColor() == BLACK)
      {
        motorLeft.pause();
        motorRight.pause();
        state++;
      }*/
      
      break;
   case 2:                    //wait to complete and rotate left
      TurnLeft(85);
      waitForMotorsStop(state + 1);
      break;
    case 3:                    //wait to complete and move forward   
      MoveForward(40,slowSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 4:                    //shoot balls 
        if(shotBalls < 6)
        {  
          if(shotBalls == 0){
            SnD.startShooting();
          }
         else{
            SnD.stopShooting();
            delay(100);
            SnD.startShooting();
         }
         MoveForward(5,slowSpeedDelay);
         shotBalls++;
         wait(1000, state);
        }
        else
        {     
          SnD.stopShooting();
          MoveForward(55,slowSpeedDelay);
          waitForMotorsStop(state + 1);
        }
      break;
    case 5:        //wait to complete and rotate left
        TurnLeft(85);
        waitForMotorsStop(state + 1);
      break;
    case 6:             //wait to complete and move forward
        MoveForward(80, slowSpeedDelay);
        waitForMotorsStop(state + 1);
      break;
    case 7:        //wait to complete and move backward
        MoveBackward(60, slowSpeedDelay);
        waitForMotorsStop(state + 1);
      break;
    case 8:
        TurnRight(85);
        waitForMotorsStop(state + 1);
      break;
    case 9:   
        MoveBackward(55,slowSpeedDelay);
        waitForMotorsStop(state + 1);
      break;
    case 10:
        SnD.ThrowNet();
        state = STATE_STOP;
      break;
    case STATE_STOP:   //stop
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
  
  motorRight.doLoop();
  motorLeft.doLoop();
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
  motorLeft.doRotationInAngle(angle);
  motorRight.doRotationInAngle(angle); 
}


void TurnRight(int angle)
{  
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.doRotationInAngle(angle);
  motorRight.doRotationInAngle(angle);
}

void ArcToLeft(int radius, int step_delay, boolean forward)
{
  motorLeft.setTargetDelay(step_delay * 2);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  if(forward) 
    motorRight.toggleDirection();
  else
    motorLeft.toggleDirection();
  motorLeft.doDistanceInCm(radius / 4);
  motorRight.doDistanceInCm(radius / 2);
}


void ArcToRight(int radius, int step_delay, boolean forward)
{
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay * 2);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  if(forward) 
    motorRight.toggleDirection();
  else
    motorLeft.toggleDirection();
  motorLeft.doDistanceInCm(radius / 2);
  motorRight.doDistanceInCm(radius / 4);
}
