#include <AccelStepper.h>

const int DirX = 2;
const int StepX = 3;
const int DirY = 4;
const int StepY = 5;

volatile bool StepState = HIGH;

const int Echo_PIN = 9;
const int Trigger_PIN = 10;

const int buzzer = 6;
const int buttonPin= 8;

AccelStepper stepperX(AccelStepper::DRIVER,StepX,DirX);
AccelStepper stepperY(AccelStepper::DRIVER,StepY,DirY);

float safeMaxDistance  ; // define safe max distance to be travel in X in mm
int stepsX ; // define steps to be travel X 
int stepsY ; // define steps to be travel Y

float getDistance(){
  digitalWrite(Trigger_PIN,HIGH);
  delay(10);
  digitalWrite(Trigger_PIN,LOW);

  float time = pulseIn(Echo_PIN,HIGH);
  float distance = (time * 0.034)/2;

  return distance;
}

void setup() {
  //set up the stepper motor PINS
  pinMode(DirX,OUTPUT);
  pinMode(StepX,OUTPUT);
  pinMode(DirY,OUTPUT);
  pinMode(StepY,OUTPUT);

  //set up the Ultrasonic PINS
  pinMode(Trigger_PIN,OUTPUT);
  pinMode(Echo_PIN,INPUT);

  //set up steppr motors speeds and accelarations
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(500);

  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(500);

}

void loop() {
  stepperX.move(stepsX);
  stepperY.move(stepsY);
  digitalWrite(StepX,StepState);
  digitalWrite(StepY,StepState);

  while ((stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) && getDistance() > safeMaxDistance)// run two motors simultaneously
  {
    stepperX.run();
    stepperY.run();
  }

  if(getDistance()<safeMaxDistance)
  {
     while (digitalRead(buttonPin) == HIGH) 
    {
      digitalWrite(buzzer,HIGH); delay(20);
      digitalWrite(buzzer,LOW); delay(20);
    }
  }

  delay(1000);
  
  StepState = !StepState;

}

