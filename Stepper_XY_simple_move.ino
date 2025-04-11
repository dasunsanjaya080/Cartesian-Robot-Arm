#include <AccelStepper.h>
int DirX = 2;
int StepX =3;
int DirY = 4;
int StepY = 5;

AccelStepper stepperX(AccelStepper::DRIVER,StepX,DirX);
AccelStepper stepperY(AccelStepper::DRIVER,StepY,DirY);

int stepsPerRevolution = 200;
float max_distance = 914.39997; // three feets in mm
float steps = (stepsPerRevolution/8)*max_distance;//for the 4 start 2mm pitch lead screw

void setup() {
  pinMode(DirX,OUTPUT);
  pinMode(StepX,OUTPUT);
  pinMode(DirY,OUTPUT);
  pinMode(StepY,OUTPUT);

  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(500);

  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(500);
}

void loop() {
  delayMicroseconds(1000);

  stepperX.move(steps); //relative steps to take stepper
  while (stepperX.distanceToGo()!=0)
  {
    stepperX.run();
  }

  delayMicroseconds(1000);

  stepperY.move(steps); //relative steps to take stepper
  while(stepperY.distanceToGo()!=0)
  {
    stepperY.run();
  }

  delayMicroseconds(1000);

  stepperY.move(0); //stepperY back to position
  while(stepperY.distanceToGo()!=0)
  {
    stepperY.run();
  }

  delayMicroseconds(1000);

  stepperX.move(0); //stepperX back to position
  while (stepperX.distanceToGo()!=0)
  {
    stepperX.run();
  }


}
