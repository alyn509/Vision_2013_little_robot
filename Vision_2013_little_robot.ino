#include <Servo.h>
#include <elapsedMillis.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include "VisionStepper.h"
#include "VisionBase.h"
#include "VisionDevices.h"
#include "VisionState.h"
#include "pins_little_robot.h"
#include "little_robot_constants.h"

#define NINETYSECONDS 3000l//88000L

#define CLASSIC_TACTIC 0
#define AGGRESSIVE_TACTIC 20
#define FRIENDLY_TACTIC 40

VisionBase base;
VisionDevices devices;
boolean ignoreSensors = false;
elapsedMillis timeUpTimer;
boolean stoppedEverything = false;

VisionState state;
int shotBalls = 0;

void setup()
{ 
  Serial.begin(9600);
  Servo test;
  test.attach(33);
  Serial.println(15);
  test.write(15);
  delay(2000);
  for (int i=20; i<=180; i+=15)
  {
    if (i%2==0)
    {
    Serial.println(i);
    test.write(i);
    delay(3000);
    }
    else
    {
    Serial.println(15);
    test.write(15);
    delay(3000);
    }
  }
  Serial.println(15);
  test.write(15);
  
  timeUpTimer = 0;
  base.init();
  devices.init();
  ignoreSensors = false;
  state.wait(1000, CLASSIC_TACTIC);
}

void loop()
{  
  switch (state)
  {
    
      //******************************************CLASSIC TACTIC**************************************************//
    case CLASSIC_TACTIC:     //move forward
      base.moveForward(30,fastSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:                    //shoot balls 
      state.wait(500, STATE_NEXT);
      break;
    case 2:        //wait to complete and rotate left
      base.moveBackward(30,fastSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:             //wait to complete and move forward
      state.wait(500, STATE_NEXT);
      break;
    case 4:             //wait to complete and move forward
      base.turnLeft(180);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 5:        //wait to complete and move backward
      state.wait(500, STATE_NEXT);
      break;
    case 6:
      base.turnRight(180);
      state.waitFor(baseStop, STATE_STOP);
      break;
    case 7:
      base.moveBackward(30,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 8:       //shoot balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveBackward(30,ultraSlowSpeedDelay * 4);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 9:
      delay(5000);
      devices.ThrowNet();
      state = STATE_STOP;
      break;
      
      //******************************************AGGRESSIVE TACTIC**************************************************//
    case AGGRESSIVE_TACTIC:
      base.moveForward(230,highPhaseDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 21:
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 22:
      base.moveForward(10,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 23:
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 24:                    //shoot half of the balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveBackward(35,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 25:        //wait to complete and rotate left
      devices.stopShooting();
      devices.stopSpinningBallTray();
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 26:             //wait to complete and move backward
      base.moveBackward(50, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 27:
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 28:             //wait to complete and move forward
      ignoreSensors = true;
      base.setSpecial();
      base.moveForward(70, slowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 29:        //wait to complete and move backward
      ignoreSensors = false;
      base.resetSpecial();
      base.moveBackward(60, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 30:
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 31:             //wait to complete and move forward
      base.moveBackward(65, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 32:                    //shoot the other half of the balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveBackward(40,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 33:        //wait to complete and throw net
      devices.stopShooting();
      devices.stopSpinningBallTray();
      devices.ThrowNet();
      state = STATE_STOP;
      break;      
    case STATE_NET:
      devices.ThrowNet();
      state = STATE_STOP;
      break;
    default:
      state.doLoop();
  }
  base.checkObstructions();
  checkForObstacle();
  base.doLoop();
  //testIfTimeUp();
}


void timeIsUpStopEverything()
{
  base.stopNow();
  state = STATE_STOP;
}

void checkForObstacle()
{
  if(base.obstructionDetected == true && ignoreSensors == false)   
    base.pause();
  else
    base.unpause();
}

boolean baseStop()
{
  return base.isStopped();
}
