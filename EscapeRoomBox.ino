//Credits:
//Example code from Arduino Libraries
//https://learn.adafruit.com/10-minute-neopixel-necklace/arduino-code
//https://www.instructables.com/LCD-1602-With-Arduino-Uno-R3/
//https://forum.arduino.cc/t/demonstration-code-for-several-things-at-the-same-time/217158
//https://forum.arduino.cc/t/how-to-program-parallel-action-using-arduino-uno/629867
//https://forums.adafruit.com/viewtopic.php?f=47&t=79735
//https://forum.arduino.cc/t/using-digital-pin-0-and-1-just-like-any-other-digital-pin/462591
//https://roboticsbackend.com/arduino-uno-pins-a-complete-practical-guide/#Arduino_Analog_pins
//https://forum.arduino.cc/t/confused-about-defining-pins/974740
//https://forum.arduino.cc/t/arduino-uno-using-analog-input-as-digital-in-out/143837
//https://www.arduino.cc/reference/en/libraries/liquidcrystal/
//https://www.arduino.cc/reference/en/language/functions/time/millis/
//https://www.arduino.cc/reference/en/language/variables/data-types/array/
//https://www.rapidtables.com/web/color/RGB_Color.html
//https://forum.arduino.cc/t/how-to-combine-array/965844
//https://forum.arduino.cc/t/how-can-i-concatenate-two-byte-arrays-into-one-array/665397
//https://www.arduino.cc/reference/en/language/functions/random-numbers/random/
//https://stackoverflow.com/questions/45034826/multiple-conditions-in-same-if-statement-in-c

#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

//Keypad Setup (Pins 2-8)
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[COLS][ROWS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte colPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte rowPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), colPins, rowPins, ROWS, COLS );

//Pixel Jewel Setup (Pin 9)
#define PIN 9
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(35, PIN);
uint8_t currentPixel = 0;

//colors setup (red, green, yellow, blue)
uint32_t colors[] = {pixels.Color(255, 0, 0), pixels.Color(0, 255, 0), pixels.Color(255, 153, 51), pixels.Color(0, 0, 255)};

//LCD Setup (Pins 10-15)
//VSS -> Ground
//VDD -> 5V
//V0 -> Variable Resitor
//RW -> Ground
//A -> 3.3V
//K -> Ground
#define RS 10
#define E 11
#define D4 12
#define D5 13
#define D6 14
#define D7 15
LiquidCrystal lcd (RS, E, D4, D5, D6, D7);

//Servo Setup
Servo servo;

//Timing Setups
unsigned long currentTime = 0;
unsigned long lightsTime = 0;
unsigned long lights1;
unsigned long lights2;
unsigned long lights3;
unsigned long lights4;
unsigned long gameTime = 300000;
uint8_t phase = 1;
bool triggered1 = false;
bool triggered2 = false;
bool triggered3 = false;

//inputVars
String input1 = "";
String input2 = "";
String input3 = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //pixel setup
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(5);

  //servo setup
  servo.attach(16);
  servo.write(0);

  //LCD Intro
  lcd.begin(16, 2);
  lcd.print("Time Traveller's");
  lcd.setCursor(0, 1);
  lcd.print("Safebox");
  delay(5000);
  lcd.clear();
  lcd.print("Your Goal:");
  lcd.setCursor(0, 1);
  lcd.print("Open the Box.");
  delay(5000);
  lcd.clear();
  lcd.print("You have 5");
  lcd.setCursor(0, 1);
  lcd.print("minutes.");
  delay(5000);
  lcd.clear();
  lcd.print("Good Luck");
  delay(5000);

  //set lights at first password combination
  uint8_t i = 0;
  uint8_t pixelCode1[21] = {1, 3, 5, 7, 8, 10, 11, 12, 14, 15, 23, 24, 26, 27, 28,29,30,31,32,33,34};
  while (i < 21) {
  pixels.setPixelColor(pixelCode1[i], colors[1]);
  i++;
  }
  pixels.show();

  //Start game time
  gameTime += millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  //loop here, switching phases as needed
  checkLoss();
  switch(phase) {
    case 1:
      Phase1();
      break;
    case 2:
      Phase2();
      break;
    case 3:
      Phase3();
      break;
    case 4:
      Win();
      break;
  }
}

//method to check if you've lost the game
void checkLoss() {
  if ((gameTime - millis()) <= 0)
  {
    pixels.clear();
    pixels.show();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Game Over!");
    delay(10000000);
  }
}

//winning method
void Win() {
  //open section
  pixels.clear();
  pixels.show();
  servo.write(130);
  delay(10000000);
}

//method to update the timer 
void updateTimer() {
  lcd.setCursor(11,1);
  String toPrint = "";
  toPrint += ((gameTime - millis())/60000);
  toPrint += ":";
  toPrint += (((gameTime - millis())/1000)%60);
  lcd.print(toPrint);
}

//method asking for input, takes the current input and a display message
void askForInput(String input, String displayMsg) {
  if (millis() >= currentTime + 250) {
    currentTime = millis();
    lcd.clear();
    lcd.print(displayMsg);
    lcd.setCursor(0, 1);
    lcd.print(input);
    updateTimer();
  }
}

//First phase, asks for current year
void Phase1() {
  String year = "2023";

  //Ask for Current Year
  askForInput(input1, "What year is it?");

  //Cut string if too long
  if (input1.length() > 4) {
    input1 = input1.substring(0,4);
  }
  
  //Check Year, if correct go to phase 2, else reset
  if (input1.length() == 4) {
    if (input1.equals(year))
    {
      lcd.clear();
      lcd.print("Correct Year");
      phase = 2;
    }
    else {
      lcd.clear();
      lcd.print("Incorrect Year");
    }
    updateTimer();
    delay(2000);
    input1 = "";
  }

  //Get input
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    input1 += key;
  }
}

//Second phase, asks for password given by lights combination
void Phase2() {
  String password = "3524";

  //Ask for Password
  askForInput(input2, "Enter Passcode");

  //Cut code if too long
  if (input2.length() > 4) {
    input2 = input2.substring(0,4);
  }
  
  //Check Code, if correct move to phase 3 and set the timings for the lights, otherwise reset
  if (input2.length() == 4) {
    if (input2.equals(password))
    {
      lcd.clear();
      lcd.print("Correct Code");
      delay(2000);
      lcd.clear();
      lcd.print("Opening");
      lightsTime = millis();
      lights1 = lightsTime + 1000;
      lights2 = lightsTime + 2000;
      lights3 = lightsTime + 3000;
      lights4 = lightsTime + 4000;
      phase = 3;
    }
    else {
      lcd.clear();
      lcd.print("Incorrect Code");
    }
    updateTimer();
    delay(2000);
    input2 = "";
  }

  //Get input
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    input2 += key;
  }
}

//Third phase, pattern of flashing lights gives password
void Phase3() {
  //Flashing lights for error
  //Ask for manual override code
  //Continue Timer
  //Variables for Phase3
  String password = "6235";
  uint8_t codedPixels[][7] = {{0, 2, 3, 4, 5, 6}, {0, 1}, {1, 3, 5, 5, 5, 5, 5}, {0, 1, 3, 4, 5}};
  uint8_t lastPixels[7] = {28,29,30,31,32,33,34};

  //Flashing Lights
  uint8_t i = 0;
  pixels.clear();
  if (millis() >= lights4) {
    triggered1 = false;
    triggered2 = false;
    triggered3 = false;
    //reset lights time
    lightsTime = millis() + 250;
    lights1 = lightsTime + 1000;
    lights2 = lightsTime + 2000;
    lights3 = lightsTime + 3000;
    lights4 = lightsTime + 4000;
    //set lights
    i = 0;
    while (i < 7) {
    pixels.setPixelColor(lastPixels[i], colors[3]);
    i++;
    }
    i = 0;
    while (i < 7) {
      pixels.setPixelColor(codedPixels[3][i] + 21, colors[3]);
      i++;
    }
    for (uint8_t y = 0; y < 3; y++)
    {
      for (uint8_t x = random(1, 7); x > 0; x--)
      {
        pixels.setPixelColor(random(0, 6) + (7*y), colors[y]);
      }
    }
    pixels.show();
  }
  //Combo3
  else if ((millis() >= lights3) && (!triggered3)) {
    triggered3 = true;
    //set lights
    i = 0;
    while (i < 7) {
    pixels.setPixelColor(lastPixels[i], colors[2]);
    i++;
    }
    i = 0;
    while (i < 7) {
      pixels.setPixelColor(codedPixels[2][i] + 14, colors[2]);
      i++;
    }
    for (uint8_t y = 0; y < 3; y++)
    {
      for (uint8_t x = random(1, 7); x > 0; x--)
      {
        if (y <= 1) {
          pixels.setPixelColor(random(0, 6) + (7*y), colors[y]);
        }
        else {
          pixels.setPixelColor(random(0, 6) + (7*(y+1)), colors[y+1]);
        }
      }
    }
    pixels.show();
  }
  //Combo2
  else if (millis() >= lights2 && triggered2 == false) {
    triggered2 = true;
    //set lights
    i = 0;
    while (i < 7) {
    pixels.setPixelColor(lastPixels[i], colors[1]);
    i++;
    }
    i = 0;
    while (i < 7) {
      pixels.setPixelColor(codedPixels[1][i] + 7, colors[1]);
      i++;
    }
    for (uint8_t y = 0; y < 3; y++)
    {
      for (uint8_t x = random(1, 7); x > 0; x--)
      {
        if (y <= 0) {
          pixels.setPixelColor(random(0, 6) + (7*y), colors[y]);
        }
        else {
          pixels.setPixelColor(random(0, 6) + (7*(y+1)), colors[y+1]);
        }
      }
    }
    pixels.show();
  }
  //Combo1
  else if (millis() >= lights1 && triggered1 == false) {
    triggered1 = true;
    //set lights
    i = 0;
    while (i < 7) {
    pixels.setPixelColor(lastPixels[i], colors[0]);
    i++;
    }
    i = 0;
    while (i < 7) {
      pixels.setPixelColor(codedPixels[0][i], colors[0]);
      i++;
    }
    for (uint8_t y = 0; y < 3; y++)
    {
      for (uint8_t x = random(1, 7); x > 0; x--)
      {
          pixels.setPixelColor(random(0, 6) + (7*(y+1)), colors[y+1]);
      }
    }
    pixels.show();
  }

  //Ask for Password
  askForInput(input3, "ERROR-OVERRIDE");

  //Cut code if too long
  if (input3.length() > 4) {
    input3 = input3.substring(0,4);
  }
  
  //Check Code if 4 Char
  if (input3.length() == 4) {
    if (input3.equals(password))
    {
      lcd.clear();
      lcd.print("ACCESS GRANTED");
      phase = 4;
    }
    else {
      lcd.clear();
      lcd.print("Incorrect Code");
    }
    updateTimer();
    delay(2000);
    input3 = "";
  }

  //Get kepad input
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    input3 += key;
  }

}