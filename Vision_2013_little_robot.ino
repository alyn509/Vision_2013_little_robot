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

#define FRONT 1
#define BACK 2
#define LEFT 3
#define RIGHT 4

#define CLASSIC_TACTIC 0
#define FRIENDLY_TACTIC 20
#define AGGRESSIVE_TACTIC 40

elapsedMillis wait_time;
int time_to_wait, state_to_set_after_wait;
VisionStepper motorLeft;
VisionStepper motorRight;
sensors_and_devices SnD;
boolean obstructionDetected = false;
boolean motorsPaused = false;
boolean ignoreSensors = false;

int state = 0;
int shotBalls = 0;
int directionMovement = 0;

void setup()
{
  SnD.init();
  //Serial.begin(9600);
  
  motorLeft.init();
  motorLeft.initDirectionForward(HIGH);
  motorLeft.initPins(enablePinLeft, directionPinLeft, stepPinLeft);
  motorLeft.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorLeft.initSizes(wheelDiameter, wheelRevolutionSteps, distanceBetweenWheels);
  
  motorRight.init();
  motorRight.initDirectionForward(LOW);
  motorRight.initPins(enablePinRight, directionPinRight, stepPinRight);
  motorRight.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorRight.initSizes(wheelDiameter, wheelRevolutionSteps, distanceBetweenWheels);
  
  //pinMode(buttonTestPin, INPUT_PULLUP);
  obstructionDetected = false;
  motorsPaused = false;
  ignoreSensors = false;
  delay(1000);
  state = 0;
}

void loop()
{  
  switch (state)
  {
    
      //******************************************CLASSIC TACTIC**************************************************//
    case CLASSIC_TACTIC:     //move forward
      MoveForward(45,mediumSpeedDelay);
      waitForMotorsStop(state + 2);
      break;
   case 1:
      //Serial.println(SnD.detectColor());
      /*if(SnD.detectColor() == BLACK)
      {
        motorLeft.pause();
        motorRight.pause();
        state++;
      }  */              //wait to complete and rotate left
      TurnLeft(180);
      waitForMotorsStop(state - 1);
      //delay(1000);
      //SnD.startSpinningBallTray();
      //MoveForward(28,ultraSlowSpeedDelay);
      //waitForMotorsStop(STATE_STOP);
      //state = STATE_STOP;
      break;
   case 2:                    //wait to complete and rotate left
      TurnLeft(90);
      waitForMotorsStop(state + 1);
      break;
    case 3:                    //wait to complete and move forward   
      MoveForward(40,mediumSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 4:                    //shoot balls 
      SnD.startShooting();  
      delay(1000);
      SnD.startSpinningBallTray();
      setStartDelays(ultraSlowStartSpeedDelay);
      MoveForward(78,ultraSlowSpeedDelay);
      waitForMotorsStop(state + 1);
        /*
        if(shotBalls < 6)
        {   
          if(shotBalls == 0){
            SnD.startShooting();
            delay(500);
          }
          
         else{
            SnD.stopShooting();
            delay(100);
            SnD.startShooting();
         }
         
         //SnD.startSpinningBallTray();
         //delay(250);
         SnD.shootBall();
         MoveForward(5,slowSpeedDelay);
         shotBalls++;
         waitForMotorsStop(state);
         
         //SnD.startSpinningBallTray();
        }
        else
        {     
          SnD.stopShooting();
          SnD.stopSpinningBallTray();
          MoveForward(55,slowSpeedDelay);
          waitForMotorsStop(state + 1);
        }
        */
      break;
    case 5:        //wait to complete and rotate left
        setStartDelays(defaultStartSpeedDelay);
        SnD.stopShooting();
        SnD.stopSpinningBallTray();
        TurnLeft(90);
        waitForMotorsStop(state + 1);
      break;
    case 6:             //wait to complete and move forward
        MoveForward(35, mediumSpeedDelay);
        waitForMotorsStop(state + 1);
      break;
    case 7:             //wait to complete and move forward
        ignoreSensors = true;
        motorLeft.setSpecial();
        motorRight.setSpecial();
        MoveForward(20, slowSpeedDelay);
        waitForMotorsStop(state + 1);
    break;
    case 8:        //wait to complete and move backward
        ignoreSensors = false;
        motorLeft.resetSpecial();
        motorRight.resetSpecial();
        MoveBackward(55, mediumSpeedDelay);
        waitForMotorsStop(state + 1);
      break;
    case 9:
        TurnRight(90);
        waitForMotorsStop(state + 1);
      break;
    case 10:   
        MoveBackward(28,mediumSpeedDelay);
        waitForMotorsStop(state + 1);
      break;
    case 11:
        delay(5000);
        SnD.ThrowNet();
        state = STATE_STOP;
      break;
      
      //******************************************AGGRESSIVE TACTIC**************************************************//
    case AGGRESSIVE_TACTIC:
      MoveForward(45,mediumSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 21:                    //wait to complete and rotate left
      TurnLeft(90);
      waitForMotorsStop(state + 1);
      break;
    case 22:                    //wait to complete and move fast to the enemy's mammoth
      setStartDelays(highPhaseDelay); 
      MoveForward(150,highPhaseDelay);
      waitForMotorsStop(state + 1);
      break;
    case 23:                    //shoot half of the balls 
      SnD.startShooting();  
      delay(1000);
      SnD.startSpinningBallTray();
      setStartDelays(ultraSlowStartSpeedDelay);
      MoveBackward(40,ultraSlowSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 24:        //wait to complete and rotate left
      setStartDelays(defaultStartSpeedDelay);
      SnD.stopShooting();
      SnD.stopSpinningBallTray();
      waitForMotorsStop(state + 1);
      break;
    case 25:             //wait to complete and move backward
      MoveBackward(35, mediumSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 26:
      TurnRight(90);
      waitForMotorsStop(state + 1);
      break;
    case 27:             //wait to complete and move forward
      ignoreSensors = true;
      motorLeft.setSpecial();
      motorRight.setSpecial();
      MoveForward(20, slowSpeedDelay);
      waitForMotorsStop(state + 1);
    break;
    case 28:        //wait to complete and move backward
      ignoreSensors = false;
      motorLeft.resetSpecial();
      motorRight.resetSpecial();
      MoveBackward(55, mediumSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 29:
      TurnRight(90);
      waitForMotorsStop(state + 1);
      break;
    case 30:             //wait to complete and move forward
      MoveForward(65, mediumSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 31:                    //shoot the other half of the balls 
      SnD.startShooting();  
      delay(1000);
      SnD.startSpinningBallTray();
      setStartDelays(ultraSlowStartSpeedDelay);
      MoveForward(40,ultraSlowSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 32:        //wait to complete and throw net
      setStartDelays(defaultStartSpeedDelay);
      SnD.stopShooting();
      SnD.stopSpinningBallTray();
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
      //if(state_to_set_after_wait == 5)
        //SnD.shootBall(); 
      break;
  }
  obstructionDetected = false;
  if (SnD.front.detect() && directionMovement == FRONT)
  {
    //Serial.println("Front detected!");
    obstructionDetected = true;
  }
  if (SnD.front2.detect() && directionMovement == FRONT)
  {
    //Serial.println("Front detected!");
    obstructionDetected = true;
  }
  if (SnD.back.detect() && directionMovement == BACK)
  {
    //Serial.println("Back detected!");
    obstructionDetected = true;
  }
  if (SnD.left.detect() && directionMovement == LEFT)
  {
    //Serial.println("Left detected!");
    obstructionDetected = true;
  }
  if (SnD.right.detect() && directionMovement == RIGHT)
  {
    //Serial.println("Right detected!");
    obstructionDetected = true;
  }
  
  if(obstructionDetected == true && ignoreSensors == false)
  {
    motorLeft.pause();
    motorRight.pause();
    motorsPaused = true;
  }
  else
  {
    if(motorsPaused == true){
      motorLeft.unpause();
      motorRight.unpause();
      motorsPaused = false;
    }
  }
  
  motorRight.doLoop();
  motorLeft.doLoop();
}

void setStartDelays(int startDelay)
{
  motorLeft.initDelays(startDelay, highPhaseDelay, maxSpeedDelay);
  motorRight.initDelays(startDelay, highPhaseDelay, maxSpeedDelay); 
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
  directionMovement = FRONT;
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.doDistanceInCm(distance);
  motorRight.doDistanceInCm(distance);
}

void MoveBackward(float distance, int step_delay)
{    
  directionMovement = BACK;
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionBackward();
  motorRight.setDirectionBackward();
  motorLeft.doDistanceInCm(distance);
  motorRight.doDistanceInCm(distance);
}

void TurnLeft(int angle)
{       
  directionMovement = LEFT;
  motorLeft.setDirectionBackward();
  motorRight.setDirectionForward();
  motorLeft.doRotationInAngle(angle);
  motorRight.doRotationInAngle(angle); 
}

void TurnRight(int angle)
{  
  directionMovement = RIGHT;
  motorLeft.setDirectionForward();
  motorRight.setDirectionBackward();
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

