  #include "VisionDevices.h"
#include "pins_little_robot.h"
#include <elapsedMillis.h>

#define STOPPED 0
#define STOPPING 1
#define RUNNING 2
#define STARTING 3

elapsedMillis waitTime;
     
const int delayActions = 4000;
const int delayBallShotsTogglesHigh = 10;
const int delayBallShotsTogglesLow = 40;
    
void sensors_and_devices::init()
{ 
  pinMode(PrepareBallPin, OUTPUT);
  digitalWrite(PrepareBallPin, LOW);
  
  pinMode(ShootBallPin, OUTPUT);
  shootingState = LOW;
  digitalWrite(ShootBallPin, shootingState);
  
  pinMode(ThrowNetPin, OUTPUT);
  digitalWrite(ThrowNetPin, LOW);
}

void sensors_and_devices::shootBall()
{
    if(waitTime>delayBallShotsTogglesHigh){
      digitalWrite(ShootBallPin, HIGH);
    }
    if(waitTime>(delayBallShotsTogglesHigh + delayBallShotsTogglesLow))
    {
      digitalWrite(ShootBallPin, LOW);
      waitTime = 0;
    }
}

void sensors_and_devices::startShooting()
{
  digitalWrite(ShootBallPin, HIGH);
}

void sensors_and_devices::stopShooting()
{
   digitalWrite(ShootBallPin, LOW);
}

void sensors_and_devices::startSpinningBallTray()
{
  shootingState = HIGH;
  digitalWrite(PrepareBallPin, shootingState);
  waitTime = 0;
}

void sensors_and_devices::stopSpinningBallTray()
{
  shootingState = LOW;
  digitalWrite(PrepareBallPin, shootingState);
}

boolean sensors_and_devices::isSpinningBallTray()
{
  return shootingState;
}

void sensors_and_devices::pauseShooting()
{
  if (isSpinningBallTray())
  {
    pausedShooting = true;
    stopSpinningBallTray();
  }
}

void sensors_and_devices::resumeShooting()
{
  if (pausedShooting)
  {
    pausedShooting = false;
    startSpinningBallTray();
  }
}

void sensors_and_devices::ThrowNet()
{
  digitalWrite(ThrowNetPin, HIGH);
  delay(delayActions);
  digitalWrite(ThrowNetPin, LOW);
}

