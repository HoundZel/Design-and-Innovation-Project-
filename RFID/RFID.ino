#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>


#define RST_PIN 9
#define SS_PIN 10


byte readCard[4];
String SwitchA = "42ED38A"; 
String SwitchB = "A6272B1A"; 
String SwitchC = "E332D6B"; 
String SwitchD = "456D58A"; 
String SwitchE = "426D38A"; 
String tagID = "";


int counterA = 0;
int counterB = 0;
int counterC = 0;
int counterD = 0;
int counterE = 0;


// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and size (16x2)


void setup()
{
  // Initiating
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight


  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);


  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);


  lcd.clear();
  lcd.print(" Access Control ");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Token:");
}


void loop()
{
  // Wait until a new tag is available
  while (getID())
  {
    lcd.clear();
    lcd.setCursor(2, 0);


    if (tagID == SwitchA)
    {


      if (counterA % 2 == 0)
      {
        digitalWrite(2, HIGH);
        lcd.print("Switch A On!");
      }
      else
      {
        digitalWrite(2, LOW);
        lcd.print("Switch A Off");
      }
      counterA++;
    }
    else if (tagID == SwitchB)
    {


      if (counterB % 2 == 0)
      {
        digitalWrite(4, HIGH);
        lcd.print("Switch B On!");
   
      }
      else
      {
        digitalWrite(4, LOW);
        lcd.print("Switch B Off");
      }
      counterB++;
    }
    else if (tagID == SwitchC)
    {


      if (counterC % 2 == 0)
      {
        digitalWrite(7, HIGH);
        lcd.print("Switch C On!");
      }
      else
      {
        digitalWrite(7, LOW);
        lcd.print("Switch C Off");
      }
      counterC++;
    }
    else if (tagID == SwitchD)
    {


      if (counterD % 2 == 0)
      {
        digitalWrite(8, HIGH);
        lcd.print("Switch D On!");
      }
      else
      {
        digitalWrite(8, LOW);
        lcd.print("Switch D Off");
      }
      counterD++;
    }
    else if (tagID == SwitchE)
    {
      if (counterE % 2 == 0)
      {
        lcd.print("Wrong Token!");
      }
      else
      {
        lcd.print("Wrong Token!");
      }
      counterE++;
    }
    else
    {
      lcd.print(" Access Denied!");
    }


    lcd.setCursor(0, 1);
    lcd.print(" ID : ");
    lcd.print(tagID);


    delay(2000);


    lcd.clear();
    lcd.print(" Access Control ");
    lcd.setCursor(0, 1);
    lcd.print("Scan Your Token:");
  }
}


// Read a new tag if available
boolean getID()
{
  // Getting ready for Reading PICCs
  if (!mfrc522.PICC_IsNewCardPresent())
  { // If a new PICC is placed on the RFID reader, continue
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  { // Since a PICC is placed, get Serial and continue
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++)
  { // The MIFARE PICCs that we use have a 4-byte UID
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
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
