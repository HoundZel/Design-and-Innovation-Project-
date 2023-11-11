#include <Servo.h>

// Define motor control pins
const int motorPin1 = 3;
const int motorPin2 = 5;
const int motorPin3 = 6;
const int motorPin4 = 9;
const int motorPin5 = 10;

// Create servo objects
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo motor5;

void setup() {
  Serial.begin(9600);
  // Attach servos to pins
  motor1.attach(motorPin1);
  motor2.attach(motorPin2);
  motor3.attach(motorPin3);
  motor4.attach(motorPin4);
  motor5.attach(motorPin5);

  // Initialize motor positions
  motor1.write(0);
  motor2.write(0);
  motor3.write(0);
  motor4.write(0);
  motor5.write(60);

  // Set switch pins as inputs
  pinMode(2, INPUT);
  pinMode(4, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(12, INPUT);
}
void loop() {

  // Read switch states (0 or 1)
  int switchAState = digitalRead(2); // Switch A pin
  int switchBState = digitalRead(4); // Switch B pin
  int switchCState = digitalRead(7); // Switch C pin
  int switchDState = digitalRead(8); // Switch D pin
  int switchEState = digitalRead(12); // Switch E pin


  // Initialize motor positions
  int motor1Position = 0;
  int motor2Position = 0;
  int motor3Position = 0;
  int motor4Position = 0;
  int motor5Position = 0;


  // Calculate desired motor positions based on switch states
  if (switchBState && !switchCState) {
    motor1Position = 60;
    motor2Position = 60;
  }
  if (switchDState) {
    motor3Position = 60;
  }
  if (switchAState && !switchEState) {
    motor4Position = 60;
    motor5Position = 0;
  }


  // Check for activation of switches C and E (C, E acts as bypass switches)
  if (switchCState && !switchEState) {
    motor3Position = 60;
  }
  if (switchEState && !switchCState) {
    motor2Position = 60;
  }


  if (switchEState && switchCState) {
    motor1Position = 60;
    motor3Position = 60;
  }


  // Set motor positions
  motor1.write(motor1Position);
  motor2.write(motor2Position);
  motor3.write(motor3Position);
  motor4.write(motor4Position);
  motor5.write(motor5Position);




}
