
#define DEBUG

//INCLUDES
#include <SPI.h> // Generic SPI interface, which is used to access the card reader
#include <SD.h> // Provides functionality for reading/writing to SD card 
# include <TMRpcm.h>

const byte phonePin = 9; // Red wire from telephone 
const byte hookPin = 8; // Green wire from telephone 
const byte lockPin = 2; // connected to relay
const byte chipSelectPin = 4;
const unsigned long debounceDelay = 5;  //ms
// The max seperation (in ms) between pulse of a digit being dialled
const unsigned long maxPulseInterval = 250; //ms
const int numDigitsInPhoneNumber = 5;
const int RELAY_PIN = A5;

//GLOBALS
//The char representation of the number dialled (+1 to allow for string-terminating character \0)
char number[numDigitsInPhoneNumber + 1];
//The digit number currently being dialled
int currentDigit;
//How many pulses have been detected for the current digit 
int pulseCount;
//States in which the telephone can be
typedef enum { ON_HOOK, OFF_HOOK, DIALLING, CONNECTED } stateType;
// Assume that the handset starts "on hook" (i.e placed on the base unit)
stateType state = ON_HOOK;
//In order to record "pulses" on the line, we keep track of the last pin reading ... 
int previousPinReading = HIGH;
// ...the time at which the pin last changed value...
unsigned long timePinChanged;
// ... and the current time 
unsigned long now = millis ();

int relayState = LOW;


void setup () {
  // Both pins will initially be set as inputs (with internal pullup resistors), although
  //may be reassigned as outputs later
  pinMode(phonePin, INPUT_PULLUP);
  pinMode(hookPin, INPUT_PULLUP);

  //The lock pin will recieve a LOW signal to release
  digitalWrite(lockPin, HIGH);
  pinMode(lockPin, OUTPUT);

    pinMode(RELAY_PIN, OUTPUT);

  //start the serial connection 
  Serial.begin(9600);
  Serial.println ("Serial connection started");
}

void loop(){

  // Is the receiver lifted off the handset?
  int hookValue = digitalRead(hookPin);

  // if the receiver is lifted, but wasn't previously
  if(hookValue == 0 && state == ON_HOOK){

    //update the puzzle state
    state = OFF_HOOK;

    //clear any information about the number we were dialling
    pulseCount = 0;
    currentDigit = 0;

    //Put the pin back into input state
    pinMode(phonePin, INPUT_PULLUP);
  }

  if(state == OFF_HOOK || state == DIALLING){

    //record the current timestamp
    now = millis();

    //test the value of the phone pin
    int pinReading = digitalRead (phonePin);

    //If the value has changed 
    if  (pinReading != previousPinReading) {

      // The user is dialling a number
      state = DIALLING;

      //"debouncing" method to ignore jittery fluctuation in readings 
      //if the time elapsed since the last time this pin was changed is only a small amount of time 
      if (now - timePinChanged < debounceDelay){
        // Don't do anything
        return;
      }

      // A HIGH signal means that a dialling puulse has been detected 
      if(pinReading == HIGH){
        pulseCount++;
      }

      //Update the stored time and reading values for further comparison
      timePinChanged = now;
      previousPinReading = pinReading;
    }

    //we've recorded a sequence of pulse, and the time since the last pulse was detected 
    //is longer than the maxPulseInterval
    if (((now - timePinChanged) >= maxPulseInterval) && pulseCount > 0) {

      //if we haven't yet dialled a complete number 
      if (currentDigit < numDigitsInPhoneNumber) {

        // The digit '0' is preresented by 10 pulses
        if (pulseCount == 10) { pulseCount = 0;}

        #ifdef DEBUG
          Serial.print (F("Digit dialled: "));
          Serial.println (pulseCount);
        #endif

        //Append the most recent digit dialled onto the end of the number array
        number[currentDigit] = pulseCount | '0';

        //Increment the counter
        currentDigit++;

        //Initialise teh next value
        number[currentDigit] = 0;
       }

       // If we've dialled the correct number of digits
       if (currentDigit == numDigitsInPhoneNumber) {

         //Print some debug information
        #ifdef DEBUG
          Serial.print (F("Number dialled: "));
          Serial.println (number);
        #endif

        //This number unlocks maglock
        if(strcmp(number, "21807")== 0) {
          #ifdef DEBUG
            Serial.println(F("Releasing lock"));
          #endif
          relayState = !relayState;
          digitalWrite(RELAY_PIN, relayState); 
          delay(30000);
        }

        //if an incorrect number was dialled
        else{
          //set the pin as OUTPUT
          #ifdef DEBUG
            Serial.println(F("Wrong Passcode"));
          #endif
          currentDigit = 0;
          delay(2500);
        }

        //set the puzzle state to complete
        state = CONNECTED;
       }

       //This digit has been processed so reset the pulse counter for the next digit 
       pulseCount = 0;
    }
  }
}