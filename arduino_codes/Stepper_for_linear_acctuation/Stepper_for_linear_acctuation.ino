#include <AccelStepper.h>

// Pins
#define STEP_PIN 2
#define DIR_PIN 3
#define TRIG_PIN 9
#define ECHO_PIN 10

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// PID Variables
// These values is to be changed
float kp = 2.0, ki = 0.05, kd = 1.0;
float setPoint = 91.44; // 3 feet in cm
float error, prevError = 0, integral = 0;
unsigned long lastTime = 0;

// Function for read the distance
float getDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout: 30ms
  return duration * 0.0343 / 2; 
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  stepper.setMaxSpeed(1000); // Max speed steps/sec
  stepper.setAcceleration(300); // Acceleration in steps/sec^2
  stepper.setSpeed(500); // Initial speed
  stepper.setCurrentPosition(0);
}

void loop() {
  float currentDistance = getDistance();
  error = setPoint - currentDistance;

  // PID calculations
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  integral += error * deltaTime;
  float derivative = (error - prevError) / deltaTime;
  prevError = error;

  float output = kp * error + ki * integral + kd * derivative;

  // Map PID output to stepper speed (optional bounding)
  float motorSpeed = constrain(output, 50, 800); // Speed in steps/sec
  stepper.setSpeed(motorSpeed);

  // If close to target, stop motion
  if (currentDistance >= setPoint - 1.0) {
    stepper.stop(); // Stop smoothly
    Serial.println("X-axis complete. Ready for Y movement.");
    while (true); // Hold or trigger next motion (Y-axis)
  }

  stepper.runSpeed(); // Continuous motion
  Serial.print("Distance: ");
  Serial.print(currentDistance);
  Serial.print(" cm | Speed: ");
  Serial.println(motorSpeed);
}
