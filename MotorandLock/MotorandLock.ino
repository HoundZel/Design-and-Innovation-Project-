#include <Wire.h>
#include <Servo.h>


Servo servo;
int angle = 0;
int redstate = 0;
int bluestate = 0;
int greenstate = 0;
int yellowstate = 0;
int correctcombo = 0;

#define lockPin 11



void setup() {
  Wire.begin();
  servo.attach(13);
  servo.write(angle);
  pinMode(lockPin, OUTPUT);


  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

}

void loop() {
  redstate = Serial.read();
  greenstate = Serial1.read();
  bluestate = Serial2.read();
  yellowstate = Serial3.read();

  if ((redstate < 1) && (greenstate < 1) && (bluestate < 1) && (yellowstate < 1)){
    correctcombo = 0;
    angle = 0;
    servo.write(angle);
    digitalWrite(lockPin, HIGH);
    
  } else if (((redstate > 0) && (greenstate > 0) && (bluestate > 0) && (yellowstate > 0)) && (redstate == 1 || greenstate == 1 || bluestate == 1 || yellowstate == 1)) {
    correctcombo = 0;
    angle = 80;
    servo.write(angle);
    digitalWrite(lockPin, HIGH);

    

  } else if ((redstate == 2) && (greenstate == 2) && (bluestate == 2) && (yellowstate == 2)) {
    correctcombo = 1;
    angle = 0;
    servo.write(angle);
    digitalWrite(lockPin, LOW);

  }
  
  Serial.print("redstate: "); Serial.print(redstate); Serial.print('\n');
  Serial.print("greenstate: "); Serial.print(greenstate); Serial.print('\n');
  Serial.print("bluestate: "); Serial.print(bluestate); Serial.print('\n');
  Serial.print("yellowstate: "); Serial.print(yellowstate); Serial.print('\n');
  Serial.println();
  delay(2000);


}
