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

VisionBase base;
VisionDevices devices;
boolean ignoreSensors = false;
elapsedMillis timeUpTimer;
boolean stoppedEverything = false;

VisionState state;
int shotBalls = 0;
int tactic;

void setup()
{ 
  Serial.begin(115200);
  tactic = CLASSIC_TACTIC;
  base.setTacticDelays(tactic);
  timeUpTimer = 0;
  base.init();
  devices.init();
  ignoreSensors = false;
  state.wait(100, tactic);
}

void loop()
{ 
  Serial.println(digitalRead(colorRedPin)); 
  switch (state)
  {
    
      //******************************************CLASSIC TACTIC**************************************************//
    case CLASSIC_TACTIC:     //      go to the mammoth
      classicStart(45,fastSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:                    //      shoot all balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveForward(35,ultraSlowSpeedDelay * 125);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:        //      stop shooting and to the edge of the black line
      devices.stopShooting();
      extraForward(35, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:             //      turn left
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 4:             //      move to the wall
      strictForward(40,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 5:             //      hit the wall
      strictForward(20,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 6:        //       get back from the wall
      strictBackward(55);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 7:        //      turn right
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;;
    case 8:        //      return to the mammoth
      base.moveBackward(35,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 9:        //      throw the net over the mammoth
      delay(5000);
      devices.ThrowNet();
      state = STATE_STOP;
      break;
      
      //******************************************AGGRESSIVE TACTIC**************************************************/
    case AGGRESSIVE_TACTIC:        //      go at fast speed at the enemy's mammoth 
      classicStart(223,highPhaseDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 21:        //      shoot half of the balls     
      base.setTacticDelays(CLASSIC_TACTIC);
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveBackward(10,ultraSlowSpeedDelay * 110);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 22:        //      stop shooting and go to the edge of the black line
      devices.stopShooting();
      devices.stopSpinningBallTray();
      lessBackward(55, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 23:        //      turn left (right in case you go backwards)
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 24:        //      go to the wall
      strictForward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 25:        //      hit the wall
      strictForward(5,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 26:        //      go back to the edge of the black line
      strictBackward(55);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 27:        //      turn right (left in case you move backwards)
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 28:        //      move backwards to your own mammoth
      extraBackward(67, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 29:        //      shoot the other half of the balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveBackward(20,ultraSlowSpeedDelay * 110);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 30:        //      stop shooting and move forward
      devices.stopShooting();
      devices.stopSpinningBallTray();
      base.moveForward(25,highPhaseDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;      
      case 31:        //     throw the net
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
  testIfTimeUp();
}

void testIfTimeUp()
{
  if(timeUpTimer == NINETYSECONDS)
    timeIsUpStopEverything();
}

void timeIsUpStopEverything()
{
  base.stopNow();
  devices.stopShooting();
  devices.stopSpinningBallTray();
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
void strictForward(int distance, int movementDelay)
{
  ignoreSensors = true;
  base.setSpecial();
  if(!base.oppositeSide)
    base.moveForward(distance, movementDelay);
  else
    base.moveBackward(distance, movementDelay);
}
void strictBackward(int distance)
{
  ignoreSensors = false;
  base.resetSpecial();
  if(!base.oppositeSide)
    base.moveBackward(distance, mediumSpeedDelay);
  else
    base.moveForward(distance, mediumSpeedDelay);
}
void classicStart(int distance, unsigned long startDelay)    // extra distance for the normal side
{
  if(!base.oppositeSide)
    base.moveForward(distance + 7, startDelay);
  else
    base.moveForward(distance, startDelay);
}
void aggroStart(int distance, unsigned long startDelay)    // extra distance for the normal side
{
  if(!base.oppositeSide)
    base.moveForward(distance, startDelay);
  else
    base.moveForward(distance + 7, startDelay);
}
void extraForward(int distance, unsigned long startDelay)    // extra distance for the opposite side
{
  if(!base.oppositeSide)
    base.moveForward(distance, startDelay);
  else
    base.moveForward(distance + 7, startDelay);
}
void extraBackward(int distance, unsigned long startDelay)    // extra distance for the opposite side
{
  if(!base.oppositeSide)
    base.moveBackward(distance, startDelay);
  else
    base.moveBackward(distance + 7, startDelay);
}
void lessBackward(int distance, unsigned long startDelay)    // extra distance for the opposite side
{
  if(!base.oppositeSide)
    base.moveBackward(distance + 9, startDelay);
  else
    base.moveBackward(distance, startDelay);
}
