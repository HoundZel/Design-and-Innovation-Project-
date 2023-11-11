#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define valve1 A0
#define valve2 A1
#define valve3 A2
#define valve4 A3

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address and size

String textToDisplay = "   Initialized successfully! Proceed to stage 3! ";
String textToDisplay2 = "  ..--- ...-- ..-. ----. -.. ....- ";
String textToDisplay21 = "  ..--- ...-- ..-. ----. -.. ....- ";
String textToDisplay3 = "   Stage 3 (^_^).Solve the Morse Code";
String textToDisplay4 = "   Goodluck!";
int scrollPosition = 0;
unsigned long lastScrollTime = 0;
String passwordd = "D,E,A,D";
String answer ="";

String code = "";
int len = 0;
char ch;
char new_char;

const int but = 2;
const int led = 13;

unsigned long pres_len = 0, rel_time, pres_time = 0, old_time_len = 0, old_pres = 0, space = 0;
int state = 0;
int unit_delay       = 250;
int min_delay = 10;

String password = "23F9D4"; // Change this to your desired 6-character password
String enteredPassword = "";
bool passwordVerified = false;
bool codeNotFound = false; // Flag to track if the Morse code does not exist

String test(int valve){
  if (valve <= 205){
    return "A";
  }
  else if(valve >205 && valve <= 410){
    return "B";
  }
  else if(valve > 410 && valve <= 615){
    return "C";
  }
  else if(valve > 615 && valve <= 820){
    return "D";
  }
  else if (valve > 820 && valve <=1023){
    return "E";
  }
}

void printer(int valve){
  if (valve <= 205){
    lcd.print("-");
    lcd.print("    ");
  }
  else if(valve >205 && valve <= 410){
    lcd.print("-");
    lcd.print("-");
    lcd.print("   ");
  }
  else if(valve > 410 && valve <= 615){
    lcd.print("-");
    lcd.print("-");
    lcd.print("-");
    lcd.print("  ");
  }
  else if(valve > 615 && valve <= 820){
    lcd.print("-");
    lcd.print("-");
    lcd.print("-");
    lcd.print("-");
    lcd.print(" ");
  }
  else if (valve > 820 && valve <=1023){
    lcd.print("-");
    lcd.print("-");
    lcd.print("-");
    lcd.print("-");
    lcd.print("-");
  }
}

void setup() {
  Serial.begin(9600);
  // Initialize LCD I2C
  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stage 2 starts!");
  delay(1000);
  lcd.clear();

  while(passwordd != answer){
    lcd.setCursor(0, 0);
    lcd.print("I:");
    printer(analogRead(valve1));

    lcd.setCursor(0, 1);
    lcd.print("J:");
    printer(analogRead(valve2));

    lcd.setCursor(8, 0);
    lcd.print("K:");
    printer(analogRead(valve3));

    lcd.setCursor(8, 1);
    lcd.print("L:");
    printer(analogRead(valve4));

    answer = String (test(analogRead(valve1))+","+test(analogRead(valve2))+","+test(analogRead(valve3))+","+test(analogRead(valve4)));
    Serial.println(answer);
    delay (250);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(3000);
  lcd.setCursor(0, 0);
  displayScrollingText();
  displayScrollingText2();
  delay(3000);
  displayScrollingText3();
  displayScrollingText21();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Goodluck!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Decrypted Code:");
  pinMode(but, INPUT_PULLUP);
  pinMode(led, OUTPUT);
}


char MakeString() {
  if (pres_len < (unit_delay * 3) && pres_len > 50) {
    return '.'; // if button press is less than 0.6 seconds, it is a dot
  } else if (pres_len > (unit_delay * 3)) {
    return '-'; // if button press is more than 0.6 seconds, it is a dash
  }
}

void Morse_decod() {
  static String morse[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
                           "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                           ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
                           "-----", ".----", "..---", "...--", "....-", ".....",
                           "-....", "--...", "---..", "----." ,"!"
                          };
  static char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  int i = 0;
  while (morse[i] != "!") {
    if (morse[i] == code) {
      new_char = char('A' + i); // Store the decoded character
      codeNotFound = false; // Reset the codeNotFound flag
      break;
    }
    i++;
  }
  if (morse[i] == "!") 
  {
    codeNotFound = true;
    Serial.println("");
    Serial.println("This code is not exist!");
  }

  for (int j = 0; j < 10; j++) 
  {
    if (code == morse[j + 26]) 
    {
      new_char = digits[j]; // Store the decoded digit
      codeNotFound = false; // Reset the codeNotFound flag
      break;
    }
  }

  code = "";
}

void loop() {
  label:
  while (digitalRead(but) == HIGH) {}
  old_pres = rel_time;
  pres_time = millis();
  digitalWrite(led, HIGH);
  while (digitalRead(but) == LOW) {}
  rel_time = millis();
  digitalWrite(led, LOW);
  pres_len = rel_time - pres_time;
  space = pres_time - old_pres;
  if (pres_len > min_delay) {
    code += MakeString();
    Serial.println(code);
  }
  while ((millis() - rel_time) < (unit_delay * 3)) {
    if (digitalRead(but) == LOW) {
      goto label;
    }
  }

  Morse_decod();

  if (!passwordVerified) 
    {
    enteredPassword += new_char; // Add the decoded character to the enteredPassword
    lcd.setCursor(0, 1);
    lcd.print(enteredPassword);
    Serial.println(enteredPassword);
    }

  if (enteredPassword.length() == 6) {
    delay(1000);
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Decrypting");
    lcd.setCursor(3,1);
    lcd.print("        ");
    lcd.setCursor(3,1);
    int loading = 0;
    while(loading < 6)
    {
    lcd.print("-");
    delay(500);
    loading += 1;
    }

  delay(2000);
    if (!codeNotFound && enteredPassword == password) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Successful");
      delay(5000);
      passwordVerified = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pass is...");
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print("9 8 3 6");
      // Do something when the password is correct
    } 
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Incorrect");
      delay(2000); // Display "Password incorrect" for 2 seconds
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Decrypted Code:");
      enteredPassword = ""; // Reset the entered password for another try
    }
    return;
  }
}

void displayScrollingText() {
  while (true) {
    if (millis() - lastScrollTime >= 250) {
      lcd.clear();
      lcd.setCursor(0, 0);

      for (int i = scrollPosition; i < (scrollPosition + 16); i++) {
        lcd.print(textToDisplay[i]);
      }

      scrollPosition++;
      if (scrollPosition >= textToDisplay.length()) {
        scrollPosition = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        delay(2000);
        return;
      }

      lastScrollTime = millis();
    }
  }
}

int scrollCounter = 0;

void displayScrollingText2() 
{
  while (scrollCounter<2) {
    if (millis() - lastScrollTime >=250) { // Reduce delay to 250 milliseconds
      lcd.clear();
      lcd.setCursor(0, 0);

      for (int i = scrollPosition; i < (scrollPosition + 16); i++) {
        lcd.print(textToDisplay2[i]);
      }

      scrollPosition++;
      if (scrollPosition >= textToDisplay2.length()) {
        scrollPosition = 0;
        scrollCounter++;
      }

      lastScrollTime = millis();
    }
  }
}

int scrollCounter2 = 0;
void displayScrollingText21() 
{
  while (scrollCounter2<1) {
    if (millis() - lastScrollTime >= 250) { // Reduce delay to 250 milliseconds
      lcd.clear();
      lcd.setCursor(0, 0);

      for (int i = scrollPosition; i < (scrollPosition + 16); i++) {
        lcd.print(textToDisplay21[i]);
      }

      scrollPosition++;
      if (scrollPosition >= textToDisplay21.length()) {
        scrollPosition = 0;
        scrollCounter2++;
      }

      lastScrollTime = millis();
    }
  }
}

void displayScrollingText3() 
{
  while (true) {
    if (millis() - lastScrollTime >= 250) {
      lcd.clear();
      lcd.setCursor(0, 0);

      for (int i = scrollPosition; i < (scrollPosition + 16); i++) {
        lcd.print(textToDisplay3[i]);
      }

      scrollPosition++;
      if (scrollPosition >= textToDisplay3.length()) {
        scrollPosition = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        delay(2000);
        return;
      }

      lastScrollTime = millis();
    }
  }
}


bool checkPassword(String input) 
{
  return input == passwordd;
}




