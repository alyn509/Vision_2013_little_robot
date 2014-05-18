
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

#define NINETYSECONDS 88000L
#define CLASSIC_TACTIC_RED 0
#define CLASSIC_TACTIC_YELLOW 10

VisionBase base;
VisionDevices devices;
boolean ignoreSensors = false;
elapsedMillis timeUpTimer;
elapsedMillis enemyTimer;
boolean stoppedEverything = false;

VisionState state;
int shotBalls = 0;
int team_color;
int tactic;
float distanceToDo = 0;

void setup()
{ 
  //Serial.begin(115200);
  digitalWrite(coolerPin, HIGH);
  tactic = CLASSIC_TACTIC;// AGGRESSIVE_TACTIC   CLASSIC_TACTIC
  base.setTacticDelays(tactic);
  timeUpTimer = 0;
  base.init();
  devices.init();
  ignoreSensors = true;
  team_color = base.oppositeSide ? 10 : 0;
  state.wait(1000, tactic + team_color);
}

void loop()
{ 
  switch (state)
  {    
      //******************************************CLASSIC TACTIC**************************************************//
    case CLASSIC_TACTIC_RED:     //      go to the mammoth
      base.setTacticDelays(FAST_START);
      base.moveForward(85,500);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:                    //      shoot all balls 
      ignoreSensors = false;
      base.setTacticDelays(CLASSIC_TACTIC);
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveBackward(35,ultraSlowSpeedDelay * 125);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:        //      stop shooting and to the edge of the black line
      devices.stopShooting();
      devices.stopSpinningBallTray();
      base.moveForward(80, mediumSpeedDelay);
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
      base.moveBackward(35,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 9:        //      throw the net over the mammoth
      delay(5000);
      devices.ThrowNet();
      state = STATE_STOP;
      break;
      //************ avoid enemy *********case 2*************
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
      state.waitFor(baseStop, STATE_NEXT);
      break;
      
    case 115:        //      throw the net over the mammoth
      delay(5000);
      devices.ThrowNet();
      state = STATE_STOP;
      break;
    //***********************//
    case CLASSIC_TACTIC_YELLOW:     //      go to the mammoth
      base.setTacticDelays(FAST_START);
      base.moveForward(75,500);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 11:             //      turn left
      base.turnLeft(180);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 12:                    //      shoot all balls 
      ignoreSensors = false;
      base.setTacticDelays(CLASSIC_TACTIC);
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveForward(35,ultraSlowSpeedDelay * 125);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 13:        //      stop shooting and to the edge of the black line
      devices.stopShooting();
      devices.stopSpinningBallTray();
      base.turnLeft(180);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 14:   
      base.moveForward(80, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 15:             //      turn left
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 16:             //      move to the wall
      ignoreSensors = true;
      base.moveForward(50,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 17:             //      hit the wall
      base.moveForward(20,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 18:        //       get back from the wall
      ignoreSensors = false;
      base.moveBackward(55,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 19:        //      turn right
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 20:        //      return to the mammoth
      base.moveForward(35,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NET);
      break;
      //************ avoid enemy *********case 2*************
    case 120:       //     
      //Serial.println("BAAAAAAAAAAAA");
      base.moveBackward(base.getDistanceMadeSoFar(),mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 121:       //     
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 122:       //     
      base.moveForward(90,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 123:       //     
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 124:       //     
      base.moveForward(130,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 125:       //     
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 126:       //     
      base.moveForward(90,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 127:       //     
      base.turnLeft(45);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 128:       //     
      base.moveForward(30,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 129:       //     
      base.turnRight(45);
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
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;;
    case 134:        //      return to the mammoth
      base.moveForward(25,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 135:        //      throw the net over the mammoth
      delay(5000);
      devices.ThrowNet();
      state = STATE_STOP;
      break;  
      //******************************************AGGRESSIVE TACTIC**************************************************/
    case AGGRESSIVE_TACTIC:        //      go at fast speed at the enemy's mammoth     
      base.setTacticDelays(AGGRESSIVE_TACTIC);
      base.moveForward(230,highPhaseDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 41:        //      shoot half of the balls     
      base.setTacticDelays(CLASSIC_TACTIC);
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveBackward(10,ultraSlowSpeedDelay * 110);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 42:        //      stop shooting and go to the edge of the black line
      devices.stopShooting();
      devices.stopSpinningBallTray();
      base.moveBackward(65, mediumSpeedDelay);
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
      base.moveForward(10,ultraSlowSpeedDelay);
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
      base.moveBackward(67, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 49:        //      shoot the other half of the balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.moveBackward(20,ultraSlowSpeedDelay * 110);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 50:        //      stop shooting and move forward
      devices.stopShooting();
      devices.stopSpinningBallTray();
      base.moveForward(25,highPhaseDelay);
      state.waitFor(baseStop, STATE_NET);
      break;      
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

void testIfTimeUp()
{
  if(timeUpTimer == NINETYSECONDS)
    timeIsUpStopEverything();
}

void timeIsUpStopEverything()
{
  digitalWrite(coolerPin, LOW);
  state = STATE_STOP;
  base.stopNow();
  devices.stopShooting();
  devices.stopSpinningBallTray();
  state = STATE_STOP;
}

void checkForObstacle()
{
  if(base.obstructionDetected == true && ignoreSensors == false && !base.isPaused())   
  {
    //Serial.println("A");
      enemyTimer = 0;
      base.pause();
  }
  else if(base.obstructionDetected == false && ignoreSensors == false && base.isPaused())
  { //Serial.println("B");
    base.unpause();
  }
  else if(enemyTimer >= 5000L && base.isPaused())
  {
    //Serial.println("C");
    switch(state.originalState)
    {
      case 2: state = 110; 
              base.stopNow();
              //Serial.println("D");
              break;
      //default:              //Serial.print(state.originalState);//Serial.print(" ");//Serial.println(state.state);break;
    }
  }
}

boolean baseStop()
{
  return base.isStopped();
}

