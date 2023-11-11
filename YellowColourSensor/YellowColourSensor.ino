#include <Wire.h>
#include <Adafruit_TCS34725.h>

int yellowstate = 0;


// Initialize the color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Define RGB LED pins
#define yellowPin 2
#define redPin 3
#define greenPin 5
#define bluePin 6

void setup() {
  // Initialize I2C communication
  Wire.begin();


  // Initialize the color sensor
  if (!tcs.begin()) {
    Serial.println("Error initializing TCS3472. Check your connections.");
    while (1);
  }

  // Initialize LED pins as OUTPUT
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(yellowPin, OUTPUT);

  // Initialize Serial communication
  Serial.begin(9600);
}

void loop() {
  // Read color values from the sensor
  uint16_t red, green, blue, clear;
  tcs.getRawData(&red, &green, &blue, &clear);

  // Calculate RGB values as percentages
  float total = 255;
  float r = red / total * 100;
  float g = green / total * 100;
  float b = blue / total * 100;
  float y = abs(r-g);

  // Print RGB values to Serial monitor
  

  // Set the LED color based on RGB values
  if (r > g && r > b && r > 50 && y > 50 && r > 75) { // Red
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(yellowPin, LOW);
    yellowstate = 1;
    Serial.write(yellowstate);
    
  } else if (g > r && g > b && g > 75) { // Green
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    digitalWrite(yellowPin, LOW);
    yellowstate = 1;
    Serial.write(yellowstate);
    
  } else if (b > r && b > g && b > 75) { // Blue
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
    digitalWrite(yellowPin, LOW);
    yellowstate = 1;
    Serial.write(yellowstate);
    
  } else if (r > b && g > b && y < 50 && r > 75) {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(yellowPin, HIGH);
    yellowstate = 2;
    Serial.write(yellowstate);
    
  }
    else {
    // Turn off all LEDs if none of the conditions are met
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(yellowPin, LOW);
    yellowstate = 0;
    Serial.write(yellowstate);
    
  }
  delay(2000); // Delay for a moment before reading again
  
  
    
  
}