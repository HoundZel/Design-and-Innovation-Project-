#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

//make custom charcs
byte Lock[] = {
  B00100,
  B01010,
  B01010,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};

byte Unlock[] = {
  B00100,
  B01010,
  B01000,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};

byte Boot[] = {
  B00010,
  B00100,
  B01000,
  B01110,
  B00010,
  B00100,
  B01000,
  B00000
};

byte Boot2[] = {
  B00000,
  B00010,
  B00100,
  B01000,
  B01110,
  B00010,
  B00100,
  B01000
};

byte Look[] = {
  B01110,
  B10001,
  B10001,
  B11101,
  B11101,
  B11101,
  B10001,
  B01110
};

byte Dance[] = {
  B01110,
  B01010,
  B01110,
  B00100,
  B11111,
  B00100,
  B01010,
  B01010
};

byte Dance2[] = {
  B01110,
  B01010,
  B01110,
  B00100,
  B11100,
  B00100,
  B01011,
  B01000
};

byte Dance3[] = {
  B01110,
  B01010,
  B01110,
  B00100,
  B00111,
  B00100,
  B11010,
  B00010
};

LiquidCrystal_I2C lcd(0x27, 16, 2);
int buzzerPin = 11;

/* Keypad setup */
const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'A', 'B'},
  {'C', 'D', 'E', 'F'}
};

uint8_t colPins[COLS] = {6, 7, 9, 8};
uint8_t rowPins[ROWS] = {2, 3, 5, 4}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void splashscreen(){

  tone(buzzerPin, 587); // D4
  delay(200);

  noTone(buzzerPin);

//start

  String welcome = "Welcome                ";
  String welcome2 = "  To                   ";
  String welcome3 = "  Galactic             ";
  String welcome4 = "Code Breaker           ";
  
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(16-i, 0);
    lcd.print(welcome);

    lcd.setCursor(16-i,1);
    lcd.print(welcome2);

    delay(200);
    lcd.clear();
  }

  for (int i = 0; i < 3; i++) {
    lcd.setCursor(i, 0);
    lcd.print(welcome3);

    lcd.setCursor(i,1);
    lcd.print(welcome4);

    delay(400);
    if(i<2){
    lcd.clear();
    }
  }
  delay(1500);

  tone(buzzerPin, 587); // D4
  delay(200);

  noTone(buzzerPin);

  lcd.clear();
}

String passgen(){
  int i = 0,j=0;
  char temp = 0;
  String password = "";

  while (password.length()<3){
    i = random(48,57);
    temp = i;
    password += temp;
  }

  j = random(65,70);
  temp = j;
  password += temp;
/*
  lcd.print(password);
  delay(1500);
  lcd.clear();
*/
  return password;
}

String guess(){
  lcd.setCursor(5, 1);
  lcd.print("[____]");
  lcd.setCursor(6, 1);
  String result = "";
  while (result.length() < 4) {
    char key = keypad.getKey();
    if (key >= '0' && key <= 'F') {
      lcd.print(key);
      tone(buzzerPin, 700); 
      delay(150);
      noTone(buzzerPin);
      result += key;
    }
  }

  lcd.setCursor(3,1);
  lcd.print("        ");
  lcd.setCursor(3,1);
  String loading = "";
  while(loading.length() < 9){
    lcd.print("-");
    delay(250);
    loading += "-";
  }
  delay(500);
  lcd.clear();

  return result;
}

bool verification(String password){
  String input = guess();
  String inputt = password;
  int i = 0;

  lcd.setCursor(4,0);
  lcd.print(input[0]);
  lcd.setCursor(6,0);
  lcd.print(input[1]);
  lcd.setCursor(8,0);
  lcd.print(input[2]);
  lcd.setCursor(10,0);
  lcd.print(input[3]);

  lcd.setCursor(4,1);
  lcd.print(inputt[0]);
  lcd.setCursor(6,1);
  lcd.print("*");
  lcd.setCursor(8,1);
  lcd.print(inputt[2]);
  lcd.setCursor(10,1);
  lcd.print("*");
  delay(1000);

  while (i < 4){
    if (input[i] == inputt[i]){
      lcd.setCursor(4+2*i,1);
      lcd.print("O");
    }
    else{
      lcd.setCursor(4+2*i,1);
      lcd.print("X");
    }
    i += 1;
  }
  delay(1000);

  if (input == inputt){
    return true;
  }
  else{
    return false;
  }

}

void fail(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("System");
  lcd.setCursor(2,1);
  lcd.print("Rebooting...");
  lcd.setCursor(3,0);
  lcd.write(byte(2));
  lcd.setCursor(12,0);
  lcd.write(byte(2));
  delay(3000);
  lcd.setCursor(3,0);
  lcd.write(byte(3));
  lcd.setCursor(12,0);
  lcd.write(byte(3));
  delay(3000);
  lcd.setCursor(3,0);
  lcd.write(byte(2));
  lcd.setCursor(12,0);
  lcd.write(byte(2));
  delay(4000);
  lcd.clear();
}

void pass(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Dial 21807");
  lcd.setCursor(0,1);
  lcd.print("To Unlock safe");
  lcd.setCursor(15,1);
  lcd.write(byte(0));
  delay(3000);
  lcd.setCursor(15,1);
  lcd.write(byte(1));
  delay(7000);
  lcd.clear();
}

void music() {
  pinMode(buzzerPin, OUTPUT);
  lcd.clear();

  tone(buzzerPin, 440); // A4
  lcd.setCursor(5,1);
  lcd.write(byte(5));
  lcd.setCursor(7,1);
  lcd.write(byte(5));
  lcd.setCursor(9,1);
  lcd.write(byte(5));
  delay(500);

  tone(buzzerPin, 494); // B4
  lcd.setCursor(5,1);
  lcd.write(byte(6));
  lcd.setCursor(7,1);
  lcd.write(byte(7));
  lcd.setCursor(9,1);
  lcd.write(byte(6));
  delay(500);

  tone(buzzerPin, 523); // C4
  lcd.setCursor(5,1);
  lcd.write(byte(7));
  lcd.setCursor(7,1);
  lcd.write(byte(6));
  lcd.setCursor(9,1);
  lcd.write(byte(7));
  delay(500);

  tone(buzzerPin, 587); // D4
  lcd.setCursor(5,1);
  lcd.write(byte(6));
  lcd.setCursor(7,1);
  lcd.write(byte(7));
  lcd.setCursor(9,1);
  lcd.write(byte(6));
  delay(500);

  tone(buzzerPin, 659); // E4
  lcd.setCursor(5,1);
  lcd.write(byte(7));
  lcd.setCursor(7,1);
  lcd.write(byte(6));
  lcd.setCursor(9,1);
  lcd.write(byte(7));
  delay(500);

  tone(buzzerPin, 698); // F4
  lcd.setCursor(5,1);
  lcd.write(byte(6));
  lcd.setCursor(7,1);
  lcd.write(byte(7));
  lcd.setCursor(9,1);
  lcd.write(byte(6));
  delay(500);

  tone(buzzerPin, 784); // G4
  lcd.setCursor(5,1);
  lcd.write(byte(7));
  lcd.setCursor(7,1);
  lcd.write(byte(6));
  lcd.setCursor(9,1);
  lcd.write(byte(7));
  delay(500);

  noTone(buzzerPin);
  lcd.setCursor(5,1);
  lcd.write(byte(5));
  lcd.setCursor(7,1);
  lcd.write(byte(5));
  lcd.setCursor(9,1);
  lcd.write(byte(5));
  delay(500);
}

void prompt(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Hint    :");
  lcd.setCursor(5,1);
  lcd.print("3A 1N");
  lcd.setCursor(8,0);
  lcd.write(byte(4));
  lcd.setCursor(9,0);
  lcd.write(byte(4));
  delay(3000);
  lcd.clear();
}

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);

  //create characters:
  lcd.createChar(0, Lock);
  lcd.createChar(1, Unlock);
  lcd.createChar(2, Boot);
  lcd.createChar(3, Boot2);
  lcd.createChar(4, Look);
  lcd.createChar(5, Dance);
  lcd.createChar(6, Dance2);
  lcd.createChar(7, Dance3);
}

void loop() {
  randomSeed(analogRead(0));

  //initalisation
  splashscreen();
  String password = passgen();

  //guessing phase (to be looped for 10 times)
  int v = 1;
  bool check = false;
  while (check == false && v < 11){
    
    if(v == 6){
      prompt();
    }

    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Attempt "+ String(v));

    check = verification(password);
    v += 1;
  }

  //if-else into one of these 
  if (check == true){
    music();
    pass();
  } 
  else{
    fail();
  }
}