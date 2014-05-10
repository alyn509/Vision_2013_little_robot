#include <elapsedMillis.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include "VisionStepper.h"
#include "VisionBase.h"
#include "VisionDevices.h"
#include "VisionState.h"

#define BLACK 100

#define CLASSIC_TACTIC 0
#define FRIENDLY_TACTIC 20
#define AGGRESSIVE_TACTIC 40

VisionBase base;
VisionDevices devices;
boolean ignoreSensors = false;

VisionState state;
int shotBalls = 0;

void setup()
{ 
  base.init();
  devices.init();
  //Serial.begin(9600);  
  
  state.wait(1000, 0);
}

void loop()
{  
  switch (state)
  {
    
      //******************************************CLASSIC TACTIC**************************************************//
    case CLASSIC_TACTIC:     //move forward
      base.setStartDelays(fastStartSpeedDelay);
      base.moveForward(150,highPhaseDelay);
      state.waitFor(baseStop, STATE_STOP);
      break;
    case 1:
      //Serial.println(devices.detectColor());
      /*if(devices.detectColor() == BLACK)
      {
        motorLeft.pause();
        motorRight.pause();
        state++;
      }  */              //wait to complete and rotate left
      base.turnLeft(180);
      state.waitFor(baseStop, STATE_LAST);
      break;
    case 2:                    //wait to complete and rotate left
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:                    //wait to complete and move forward   
      base.moveForward(40,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 4:                    //shoot balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.setStartDelays(ultraSlowStartSpeedDelay);
      base.moveForward(78,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 5:        //wait to complete and rotate left
        base.setStartDelays(defaultStartSpeedDelay);
        devices.stopShooting();
        devices.stopSpinningBallTray();
        base.turnLeft(90);
        state.waitFor(baseStop, STATE_NEXT);
      break;
    case 6:             //wait to complete and move forward
        base.moveForward(35, mediumSpeedDelay);
        state.waitFor(baseStop, STATE_NEXT);
      break;
    case 7:             //wait to complete and move forward
        base.ignoreSensors = true;
        base.setSpecial();
        base.moveForward(20, slowSpeedDelay);
        state.waitFor(baseStop, STATE_NEXT);
    break;
    case 8:        //wait to complete and move backward
        base.ignoreSensors = false;
        base.resetSpecial();
        base.moveBackward(55, mediumSpeedDelay);
        state.waitFor(baseStop, STATE_NEXT);
      break;
    case 9:
        base.turnRight(90);
        state.waitFor(baseStop, STATE_NEXT);
      break;
    case 10:   
        base.moveBackward(28,mediumSpeedDelay);
        state.waitFor(baseStop, STATE_NEXT);
      break;
    case 11:
        delay(5000);
        devices.ThrowNet();
        state = STATE_STOP;
      break;
      
      //******************************************AGGRESSIVE TACTIC**************************************************//
    case AGGRESSIVE_TACTIC:
      base.moveForward(45,mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 21:                    //wait to complete and rotate left
      base.turnLeft(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 22:                    //wait to complete and move fast to the enemy's mammoth
      base.setStartDelays(highPhaseDelay); 
      base.moveForward(150,highPhaseDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 23:                    //shoot half of the balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.setStartDelays(ultraSlowStartSpeedDelay);
      base.moveBackward(40,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 24:        //wait to complete and rotate left
      base.setStartDelays(defaultStartSpeedDelay);
      devices.stopShooting();
      devices.stopSpinningBallTray();
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 25:             //wait to complete and move backward
      base.moveBackward(35, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 26:
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 27:             //wait to complete and move forward
      base.ignoreSensors = true;
      base.setSpecial();
      base.moveForward(20, slowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
    break;
    case 28:        //wait to complete and move backward
      base.ignoreSensors = false;
      base.resetSpecial();
      base.moveBackward(55, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 29:
      base.turnRight(90);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 30:             //wait to complete and move forward
      base.moveForward(65, mediumSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 31:                    //shoot the other half of the balls 
      devices.startShooting();  
      delay(1000);
      devices.startSpinningBallTray();
      base.setStartDelays(ultraSlowStartSpeedDelay);
      base.moveForward(40,ultraSlowSpeedDelay);
      state.waitFor(baseStop, STATE_NEXT);
      break;
    case 32:        //wait to complete and throw net
      base.setStartDelays(defaultStartSpeedDelay);
      devices.stopShooting();
      devices.stopSpinningBallTray();
      devices.ThrowNet();
      state = STATE_STOP;
      break;
  }  
  base.doLoop();
}

boolean baseStop()
{
  return base.isStopped();
}
