
#include <elapsedMillis.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include <Servo.h> 
#include "VisionStepper.h"
#include "VisionBase.h"
#include "VisionDevices.h"
#include "VisionState.h"
#include "pins_little_robot.h"
#include "little_robot_constants.h"

#define NINETYSECONDS 89000L

#define CLASSIC_TACTIC_RED 0
#define CLASSIC_TACTIC_YELLOW 20

#define SAFE_TACTIC_RED 200
#define SAFE_TACTIC_YELLOW 220

#define AGGRESSIVE_TACTIC_RED 40
#define AGGRESSIVE_TACTIC_YELLOW 60

VisionBase base;
VisionDevices devices;
boolean ignoreSensors = false;
elapsedMillis timeUpTimer;
elapsedMillis extra;
elapsedMillis enemyTimer, enemyThere;
boolean stoppedEverything = false;

VisionState state;
int shotBalls = 0;
int team_color;
int tactic;
float distanceToDo = 0;

void setup()
{ 
  //Serial.begin(115200);
  tactic = HOMOLOGATION;    //    AGGRESSIVE_TACTIC     CLASSIC_TACTIC     SAFE_TACTIC     HOMOLOGATION
  //base.setTacticDelays(tactic);
  timeUpTimer = 0;
  base.init();
  devices.init();
  devices.startShooting();  
  ignoreSensors = true; 
  team_color = base.oppositeSide ? 20 : 0;
  team_color = (team_color == HOMOLOGATION) ? 0 : team_color;
  state.wait(100, tactic + team_color);
}

void loop()
{ 
  switch (state)
  {  
    case HOMOLOGATION:  
      ignoreSensors = false;
      base.setTacticDelays(CLASSIC_TACTIC);
      base.moveForward(80,ultraSlowSpeedDelay/20);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case -199:                    //      shoot all balls 
      devices.startSpinningBallTray();
      base.moveBackward(35,ultraSlowSpeedDelay * 125);
      state.waitFor(baseStop, STATE_STOP);
      break;
      //******************************************CLASSIC TACTIC**************************************************//
    case CLASSIC_TACTIC_RED:     //      go to the mammoth
      base.setTacticDelays(FAST_START);
      base.moveForward(87,fastSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:                    //      shoot all balls 
      ignoreSensors = false;
      base.setTacticDelays(CLASSIC_TACTIC);
      devices.startSpinningBallTray();
      base.moveBackward(35,ultraSlowSpeedDelay * 125);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:        //      stop shooting and to the edge of the black line
      devices.stopSpinningBallTray();
      base.moveForward(70, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:             //      turn left
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 4:             //      move to the wall
      ignoreSensors = true;
      base.moveForward(50,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 5:             //      hit the wall
      base.moveForward(20,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 6:        //       get back from the wall
      ignoreSensors = false;
      base.moveBackward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 7:        //      turn right
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;;
    case 8:        //      return to the mammoth
      base.moveBackward(40,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_STOP);
      break;
      //************ avoid enemy ********* case 2 ************
    case 100:       //     
      base.moveBackward(base.getDistanceMadeSoFar(),mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 101:       //     
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 102:       //     
      base.moveForward(90,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 103:       //     
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 104:       //     
      base.moveForward(130,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 105:       //     
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 106:       //     
      base.moveForward(90,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 107:       //     
      base.turnLeft(45);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 108:       //     
      base.moveForward(30,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 109:       //     
      base.turnRight(45);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 110:             //      move to the wall
      base.moveForward(40,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 111:             //      hit the wall
      base.moveForward(10,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 112:        //       get back from the wall
      base.moveBackward(60,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 113:        //      turn right
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;;
    case 114:        //      return to the mammoth
      base.moveForward(25,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_STOP);
      break;
      
    //******+++++****** SAME TACTIC -> OTHER SIDE COLOR ******+++++****** //
    
    case CLASSIC_TACTIC_YELLOW:     //      go to the mammoth
      base.setTacticDelays(FAST_START);
      base.moveForward(75,fastSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 21:             //      turn left
      base.turnLeft(180);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 22:                    //      shoot all balls 
      ignoreSensors = false;
      base.setTacticDelays(CLASSIC_TACTIC);
      devices.startSpinningBallTray();
      base.moveForward(35,ultraSlowSpeedDelay * 125);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 23:        //      stop shooting and to the edge of the black line
      devices.stopSpinningBallTray();
      base.turnLeft(180);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 24:   
      base.moveForward(80, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 25:             //      turn left
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 26:             //      move to the wall
      ignoreSensors = true;
      base.moveForward(50,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 27:             //      hit the wall
      base.moveForward(20,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 28:        //       get back from the wall
      ignoreSensors = false;
      base.moveBackward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 29:        //      turn right
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 30:        //      return to the mammoth
      base.moveForward(40,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_STOP);
      break;
      //++++++++++++++++++++++++++++++++++ avoid enemy  -----------------------  case 2 & 24 ++++++++++++++++++++++++++++++++++
    case 120:       //     
      base.moveBackward(base.getDistanceMadeSoFar(),mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 121:       //     
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 122:       //     
      base.moveForward(90,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 123:       //     
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 124:       //     
      base.moveForward(130,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 125:       //     
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 126:       //     
      base.moveForward(90,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 127:       //     
      base.turnRight(45);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 128:       //     
      base.moveForward(30,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 129:       //     
      base.turnLeft(45);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 130:             //      move to the wall
      base.moveForward(40,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 131:             //      hit the wall
      base.moveForward(10,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 132:        //       get back from the wall
      base.moveBackward(60,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 133:        //      turn right
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;;
    case 134:        //      return to the mammoth
      base.moveForward(25,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_STOP);
      break;
     
       //******************************************SAFE TACTIC**************************************************//
    case SAFE_TACTIC_RED:     //      go to the mammoth
      base.setTacticDelays(FAST_START);
      base.moveForward(122,fastSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 201:             //      turn left
      base.setTacticDelays(CLASSIC_TACTIC);
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 202:             //      move to the wall
      base.moveForward(50,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 203:             //      hit the wall
      base.moveForward(20,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 204:        //       get back from the wall
      ignoreSensors = false;
      base.moveBackward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 205:        //      turn right
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;;
    case 206:        //      return to the mammoth
      base.moveBackward(40,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 207:                    //      shoot all balls 
      devices.startSpinningBallTray();
      base.moveBackward(35,ultraSlowSpeedDelay * 125);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 208:        //      stop shooting and to the edge of the black line
      devices.stopSpinningBallTray();
      base.moveForward(35, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_STOP);
      break;
      
    //******+++++****** SAME TACTIC -> OTHER SIDE COLOR ******+++++****** //
    
    case SAFE_TACTIC_YELLOW:     //      go to the mammoth
      base.setTacticDelays(FAST_START);
      base.moveForward(122,fastSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;    
    case 221:             //      turn left
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 222:             //      move to the wall
      base.moveForward(50,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 223:             //      hit the wall
      base.moveForward(20,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 224:        //       get back from the wall
      ignoreSensors = false;
      base.moveBackward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 225:        //      turn right
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 226:        //      return to the mammoth
      base.moveForward(40,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;    
    case 227:                    //      shoot all balls 
      ignoreSensors = false;
      base.setTacticDelays(CLASSIC_TACTIC);
      devices.startSpinningBallTray();
      base.moveForward(35,ultraSlowSpeedDelay * 125);
      state.waitFor(baseStop, STATE_NEXT);
      break; 
    case 228:        //      stop shooting and to the edge of the black line
      devices.stopSpinningBallTray();
      base.moveBackward(35, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_STOP);
      break;
     
      //******************************************AGGRESSIVE TACTIC**************************************************/
    case AGGRESSIVE_TACTIC_RED:        //      go at fast speed at the enemy's mammoth     
      base.setTacticDelays(AGGRESSIVE_TACTIC);
      base.moveForward(207,highPhaseDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 41:        //      shoot half of the balls     
      base.setTacticDelays(CLASSIC_TACTIC);
      devices.startSpinningBallTray();
      base.moveBackward(5,ultraSlowSpeedDelay * 110);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 42:        //      stop shooting and go to the edge of the black line
      devices.stopSpinningBallTray();
      base.moveBackward(33, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 43:        //      turn left (right in case you go backwards)
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 44:        //      go to the wall
      base.moveForward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 45:        //      hit the wall
      base.moveForward(20,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 46:        //      go back to the edge of the black line
      base.moveBackward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 47:        //      turn right (left in case you move backwards)
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 48:        //      move backwards to your own mammoth
      base.moveBackward(68, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 49:        //      shoot the other half of the balls 
      devices.startSpinningBallTray();
      base.moveBackward(25,ultraSlowSpeedDelay * 110);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 50:        //      stop shooting and move forward
      devices.stopSpinningBallTray();
      base.moveForward(25,highPhaseDelay);
      state.waitFor(baseStop, STATE_STOP);
      break;      
      
    //******+++++****** SAME TACTIC -> OTHER SIDE COLOR ******+++++****** //
      
    case AGGRESSIVE_TACTIC_YELLOW:        //      go at fast speed at the enemy's mammoth     
      base.setTacticDelays(AGGRESSIVE_TACTIC);
      base.moveForward(200,highPhaseDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 61:             //      turn left
      base.setTacticDelays(CLASSIC_TACTIC);
      base.turnLeft(180);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 62:        //      shoot half of the balls     
      base.setTacticDelays(CLASSIC_TACTIC);
      devices.startSpinningBallTray();
      base.moveForward(5,ultraSlowSpeedDelay * 110);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 63:        //      stop shooting and go to the edge of the black line
      devices.stopSpinningBallTray();
      base.moveForward(40, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 64:        //      turn left (right in case you go backwards)
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 65:        //      go to the wall
      base.moveForward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 66:        //      hit the wall
      base.moveForward(20,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 67:        //      go back to the edge of the black line
      base.moveBackward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 68:        //      turn right (left in case you move backwards)
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 69:        //      move backwards to your own mammoth
      base.moveForward(80, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 70:        //      shoot the other half of the balls 
      devices.startSpinningBallTray();
      base.moveForward(25,ultraSlowSpeedDelay * 110);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 71:        //      stop shooting and move forward
      devices.stopSpinningBallTray();
      base.moveBackward(25,highPhaseDelay);
      state.waitFor(baseStop, STATE_STOP);
      break;      
      
      //++++++++++++++++++++++++++++++++++  pull back  ----------------------- aggro failed ++++++++++++++++++++++++++++++++++
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
  if(timeUpTimer > NINETYSECONDS && extra == 3000)
    devices.ThrowNet();
  
}

void timeIsUpStopEverything()
{
  extra = 0;
  devices.stopShooting();
  state = STATE_STOP;
  base.stopNow();
}

void checkForObstacle()
{
  if(base.obstructionDetected == true && ignoreSensors == false && !base.isPaused())   
  {
      enemyTimer = 0;
      base.pause();
      devices.pauseSpinningBallTray();
      enemyThere = 0;
  }
  else if(base.obstructionDetected == false && ignoreSensors == false && base.isPaused() && enemyThere > 500L)
  {
    base.unpause();
    devices.resumeSpinningBallTray();
  }
  else if(enemyTimer >= 5000L && base.isPaused())
  {
    switch(state.originalState)
    {
      case 2: state = 100; 
              base.stopNow();
              break;
      case 24: state = 120; 
              base.stopNow();
              break;
    }
  }
}

boolean baseStop()
{
  return base.isStopped();
}

