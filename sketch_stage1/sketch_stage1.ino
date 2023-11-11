#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int buttonPins[] = {22, 23, 24, 25, 30, 31, 32, 33 };
const int powerSwitchPin = 11;
const int finalLED = 10;
const int checkButtonPin = 13;

// Define the correct sequence of button presses
const int correctSequence[] = {22,30,32,25,24,31,23,33};
const int sequenceLength = sizeof(correctSequence) / sizeof(correctSequence[0]);
int sequenceIndex = 0;
bool sequenceInProgress = false;
bool powerOn = false;

bool ledStates[] = {false, false, false, false,false,false,false,false};
bool finalLEDState = false;

void setup() {
  // Start Serial Communication
  Serial.begin(9600);

  // Initialize LCD I2C
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power OFF");
  delay(1000);

  // Initialize LED pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Initialize push button pins as inputs with pull-up resistors
  for (int i = 0; i < 8; i++) 
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Initialize the 9th LED pin
  pinMode(finalLED, OUTPUT);

  // Initialize the power switch pin
  pinMode(powerSwitchPin, INPUT_PULLUP);

  // Initialize the sequence check button pin
  pinMode(checkButtonPin, INPUT_PULLUP);
}

void loop() {
  // Read the state of the power switch
  bool powerSwitchState = digitalRead(powerSwitchPin);

  for (int i = 0; i < 8; i++) {
    int buttonState = digitalRead(buttonPins[i]);
    
    if (buttonState == LOW) {
      Serial.print("Button Pressed: ");
      Serial.println(buttonPins[i]);
    }
  }

  if (!powerOn && powerSwitchState) 
  {
    // Power switch is turned on
    powerOn = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Power ON");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press Sequence");
    delay(1000);

  } else if (powerOn && !powerSwitchState) 
  {
    // Power switch is turned off
    powerOn = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Power OFF");

    // Turn off all LEDs, reset the sequence, and clear the LCD
    for (int i = 0; i < 8; i++) 
    {
      ledStates[i] = false;
      digitalWrite(ledPins[i], LOW);
    }
    sequenceIndex = 0;
    sequenceInProgress = false;
    finalLEDState = false;
    digitalWrite(finalLED, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Power OFF");
    delay(1000);
  }

  if (powerOn) 
  {
    // Read push button states and handle toggling LEDs
    for (int i = 0; i < 8; i++) 
    {
      int buttonState = digitalRead(buttonPins[i]);

      if (buttonState == LOW) 
      {
        if (!sequenceInProgress) 
        {
          // New sequence started
          sequenceInProgress = true;
          sequenceIndex = 0;
        }

        if (buttonPins[i] == correctSequence[sequenceIndex]) 
        {
          // Correct button press detected
          ledStates[i] = !ledStates[i];
          digitalWrite(ledPins[i], ledStates[i] ? HIGH : LOW);
          sequenceIndex++;

          if (sequenceIndex == sequenceLength) 
          {
            // Correct sequence completed
            sequenceInProgress = false;
            // Turn on the 9th LED
            finalLEDState = true;
            digitalWrite(finalLED, HIGH);
            delay(5000); // 5 seconds
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Malfunction!");
            delay(5000);
            Stage2();
            break;
          }
        } else 
          {
            // Incorrect button press, reset sequence
            resetAll();
          }
      }
  


    // Check if the sequence check button is pressed
    int checkButtonState = digitalRead(checkButtonPin);
    Serial.print("Check Button State: ");
    Serial.println(checkButtonState); // Debugging statement

    if (checkButtonState == LOW) 
    {
      // Check the sequence
      bool isCorrect = true;
      for (int i = 0; i < sequenceLength; i++) 
      {
        if (correctSequence[i] != buttonPins[i]) 
        {
          isCorrect = false;
          break;
        }
      }

      if (isCorrect) 
      {
        // Correct sequence, turn on the 9th LED
        finalLEDState = true;
        digitalWrite(finalLED, HIGH);
        delay(5000); // 5 seconds
        resetAll();
      } else 
      {
        // Incorrect sequence, reset
        resetAll();
      }
    }
  }
}


    // Check if the sequence check button is pressed
    int checkButtonState = digitalRead(checkButtonPin);
    Serial.print("Check Button State: ");
    Serial.println(checkButtonState);

    if (checkButtonState == LOW) {
      // Check the sequence
      bool isCorrect = true;
      for (int i = 0; i < sequenceLength; i++) {
        if (correctSequence[i] != ledStates[i]) {
          isCorrect = false;
          break;
        }
      }

      Serial.print("isCorrect: ");
      Serial.println(isCorrect);

      if (isCorrect) {
        // Correct sequence, turn on the 9th LED
        finalLEDState = true;
        digitalWrite(finalLED, HIGH);
        delay(5000); // 5 seconds
        resetAll();
      } else {
        // Incorrect sequence, reset
        resetAll();
      }
    }
}

void resetAll() {
  // Turn off all LEDs, reset the sequence, and clear the LCD
  for (int i = 0; i < 8; i++) 
  {
    ledStates[i] = false;
    digitalWrite(ledPins[i], LOW);
  }
  sequenceIndex = 0;
  sequenceInProgress = false;
  finalLEDState = false;
  digitalWrite(finalLED, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Incorrect!");
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press sequence");
  delay(1000);
}

void Stage2()
{
  for (int i = 0; i < 8; i++)
   {
    ledStates[i] = false;
    digitalWrite(ledPins[i], LOW);
   }
  sequenceIndex = 0;
  sequenceInProgress = false;
  finalLEDState = false;
  digitalWrite(finalLED, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Proceed to");
  lcd.setCursor(0,1);
  lcd.print("Stage 2!");
  delay(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power ON");
  delay(3000);
  resetAll();
}