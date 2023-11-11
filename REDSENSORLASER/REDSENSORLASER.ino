#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Servo.h>


const int motorPin1 = 3;
Servo motor1;


// Initialize the color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


void setup() {
  // Initialize I2C communication
  Wire.begin();
  motor1.attach(motorPin1);


  // Initialize the color sensor
  if (!tcs.begin()) {
    Serial.println("Error initializing TCS3472. Check your connections.");
    while (1);
  }


  // Initialize LED pins as OUTPUT
  pinMode(3, OUTPUT);
 


  // Initialize Serial communication
  Serial.begin(9600);
}


void loop() {
  // Read colour values from the sensor
  uint16_t red, green, blue, clear;
  tcs.getRawData(&red, &green, &blue, &clear);


  // Calculate RGB values as percentages
  float total = 255;
  float r = red / total * 100;
  float g = green / total * 100;
  float b = blue / total * 100;
  float y = abs(r-g);


  // Set the LED color based on RGB values
  if (r > g && r > b && r > 30 && y > 30 && r > 30) { // Red
  motor1.write(180);
  }


  delay(500); // Delay for a moment before reading again
 
}
