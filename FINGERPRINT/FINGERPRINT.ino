#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>




volatile int finger_status = -1;
bool matchFound = false; // Flag to track if a match is found




SoftwareSerial mySerial(2, 3); // TX/RX on fingerprint sensor




Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);




LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and size (16x2)




void setup()
{
 
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(5, 0);
  lcd.print("Weapon");
  lcd.setCursor(0, 1);
  lcd.print("Access System A");
  pinMode(4, OUTPUT);




  mySerial.begin(9600);
  Serial.begin(9600);
  while (!Serial); // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");




  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (finger.verifyPassword())
  {
    lcd.setCursor(0, 1);
    lcd.print("Access System A");
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("Sensor not found");
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
    {
      delay(1);
    }
  }
  finger.getTemplateCount();
  Serial.print("Sensor contains ");
  Serial.print(finger.templateCount);
  Serial.println(" templates");
}


void loop() 
{
  if (!matchFound) // Check if a match has not been found yet
  {
    finger_status = getFingerprintIDez();
    if (finger_status != -1 && finger_status != -2)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Weapon Activated");
      lcd.setCursor(2, 1);
      lcd.print("by Commander");
      matchFound = true; // Set the match found flag
      digitalWrite(4, HIGH);
    }


    else
    {
      if (finger_status == -2)
      {
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Access");
        lcd.setCursor(5, 1);
        lcd.print("Denied");
      }
    }
  }
  delay(50); // don't need to run this at full speed.
}




// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != 2)
  {
    Serial.println(p);
  }
  if (p != FINGERPRINT_OK)
    return -1;




  p = finger.image2Tz();
  if (p != 2)
  {
    Serial.println(p);
  }
  if (p != FINGERPRINT_OK)
    return -1;




  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
    return -2;




  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}
