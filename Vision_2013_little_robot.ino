#include <elapsedMillis.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include "VisionStepper.h"
#include "VisionBase.h"
#include "VisionDevices.h"
#include "pins_little_robot.h"
#include "little_robot_constants.h"

#define STATE_NET -4
#define STATE_STOP -1
#define STATE_WAIT -2
#define STATE_WAIT_MOTORS_STOP -3
#define BLACK 100

#define CLASSIC_TACTIC 0
#define FRIENDLY_TACTIC 20
#define AGGRESSIVE_TACTIC 40

elapsedMillis wait_time, waitSlow, ninetySecondsTimer;
int time_to_wait, state_to_set_after_wait;
VisionBase base;

sensors_and_devices devices;

boolean motorsPaused = false;
boolean ignoreSensors = false;
boolean stoppedEverything = false;

int state = 0;
int shotBalls = 0;

void setup()
{
  ninetySecondsTimer = 0;
  
  devices.init();
  //Serial.begin(9600);
  
  base.init();
  //pinMode(buttonTestPin, INPUT_PULLUP);
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
 
      state = 1;
      break;
    case 1:
      //Serial.println(devices.detectColor());
     /* if (motorLeft.stepsMadeSoFar / motorLeft.stepCmRatio > 10)*/
         state = 2;
      break;
    case 2:                    //wait to complete and rotate left
      waitForMotorsStop(STATE_STOP);
      break;
    case 3:                    //wait to complete and move forward  
      ignoreSensors = false;
      base.moveForward(200,fastSpeedDelay);//57,mediumSpeedDelay);
      waitForMotorsStop(4);
      break;
    case 4:                    //shoot balls 
      devices.startShooting();  
      delay(1000);
      ignoreSensors = true;
      devices.startSpinningBallTray();
      base.moveForward(20,ultraSlowSpeedDelay);
      waitForMotorsStop(5);
      break;
    case 5:        //wait to complete and rotate left
        //devices.stopShooting();
        //devices.stopSpinningBallTray();
        base.moveForward(45, mediumSpeedDelay);
        waitForMotorsStop(6);
      break;
    case 6:             //wait to complete and move forward
        base.turnLeft(90);
        waitForMotorsStop(7);
      break;
    case 7:             //wait to complete and move forward
        //ignoreSensors = false;
        ignoreSensors = true;
        base.setSpecial();
        base.moveForward(50, slowSpeedDelay);
        waitForMotorsStop(8);
    break;
    case 8:        //wait to complete and move backward
        ignoreSensors = false;
        base.resetSpecial();
        base.moveBackward(55, mediumSpeedDelay);
        waitForMotorsStop(9);
      break;
    case 9:
        base.turnRight(90);
        waitForMotorsStop(10);
      break;
    case 10:   
        base.moveBackward(28,mediumSpeedDelay);
        waitForMotorsStop(11);
      break;
    case 11:
        delay(5000);
        devices.ThrowNet();
        state = STATE_STOP;
        
      //******************************************AGGRESSIVE TACTIC**************************************************//
    case AGGRESSIVE_TACTIC:
      base.moveForward(45,mediumSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 21:                    //wait to complete and rotate left
      base.turnLeft(90);
      waitForMotorsStop(state + 1);
      break;
    case 22:                    //wait to complete and move fast to the enemy's mammoth
      base.moveForward(150,highPhaseDelay);
      waitForMotorsStop(state + 1);
      break;
    case 23:                    //shoot half of the balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveBackward(40,ultraSlowSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 24:        //wait to complete and rotate left
      devices.stopShooting();
      devices.stopSpinningBallTray();
      waitForMotorsStop(state + 1);
      break;
    case 25:             //wait to complete and move backward
      base.moveBackward(35, mediumSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 26:
      base.turnRight(90);
      waitForMotorsStop(state + 1);
      break;
    case 27:             //wait to complete and move forward
      ignoreSensors = true;
      base.setSpecial();
      base.moveForward(20, slowSpeedDelay);
      waitForMotorsStop(state + 1);
    break;
    case 28:        //wait to complete and move backward
      ignoreSensors = false;
      base.resetSpecial();
      base.moveBackward(55, mediumSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 29:
      base.turnRight(90);
      waitForMotorsStop(state + 1);
      break;
    case 30:             //wait to complete and move forward
      base.moveForward(65, mediumSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 31:                    //shoot the other half of the balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveForward(40,ultraSlowSpeedDelay);
      waitForMotorsStop(state + 1);
      break;
    case 32:        //wait to complete and throw net
      devices.stopShooting();
      devices.stopSpinningBallTray();
      devices.ThrowNet();
      state = STATE_STOP;
      break;
      
    case STATE_NET:
      devices.ThrowNet();
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
      if (base.isStopped())
      {
        state = state_to_set_after_wait;
      }
      //if(state_to_set_after_wait == 5)
        //devices.shootBall(); 
      break;
  }
  checkForObstacle();
  base.doLoop();
  checkIfTimeUp();
}

void checkForObstacle()
{
  base.checkObstructions();
  if(base.obstructionDetected == true && ignoreSensors == false)
  {
    base.pause();
    devices.pauseShooting();
    motorsPaused = true;
  }
  else
  {
    if(motorsPaused == true){
      base.unpause();
      devices.resumeShooting();
      motorsPaused = false;
    }
  }
}

void checkIfTimeUp()
{  
  if (ninetySecondsTimer > 10000L && !stoppedEverything)//90 sec
  {
    base.stopNow();
    devices.stopShooting();
    state = STATE_NET;
    stoppedEverything = true;
  }
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
