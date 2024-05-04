#include <TimerFreeTone.h>
#define TONE_PIN 6 // Pin you have speaker/piezo connected to (be sure to include a 100 ohm resistor).
// Melody (liberated from the toneMelody Arduino example sketch by Tom Igoe).
int melody[] = { 1000,1000,500,400,500,400,500,400,1500,1500 };
int duration[] = { 700,700,700,700,700,700,700,700,700,700 };


#include <SoftwareSerial.h>
#include <RTClibExtended.h>
RTC_DS3231 RTC;      //we are using the DS3231 RTC
DateTime Mydatetime;

#include <EEPROM.h>

#include <Wire.h>
#include <SPI.h> //i2c and the display libraries
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)




int pushVal = 0;
int val;
int val2;
int addr = 0;


//Bool pushPressed; //flag to keep track of push button state
int pushpressed = 0;
const int ledPin = 6;
int ledState = LOW;
int Signal = 0;
int buzz = 6;

int push1state, push2state, push3state, stopinState;
int push1Flag, push2Flag, push3Flag = false; // push button flags

//pins connected to buttons.
int push1pin = 5;
int push2pin = 4;
int push3pin = 3;
int stopPin = 2;

//for buzzer
long previousMillis = 0;
long interval = 500; // buzzing interval
unsigned long currentMillis;
//..

//set reminder change time, make sure it's in 24 hours format
int buzz8amHH = 22; //hours
int buzz8amMM = 19; //minutes
int buzz8amSS = 00; //seconds

int buzz2pmHH = 13; //hours
int buzz2pmMM = 10; //minutes
int buzz2pmSS = 00; //seconds

int buzz8pmHH = 20; //hours
int buzz8pmMM = 00; //minutes
int buzz8pmSS = 00; //seconds

int nowHr, nowMin, nowSec; //to show current time

int MM = 30;//alarm should not go past this munites
void setup() {
  //Serial.begin(9600);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  delay(1000);
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));   //set RTC date and time to COMPILE time
  delay(10);
  display.clearDisplay();
  display.display();
  delay(1000);
   testdrawcircle();    // Draw circles (outlines)
  testscrolltext();    // Draw scrolling text
welcome();
  
  pinMode(push1pin, INPUT_PULLUP); // define push button pins type
  pinMode(push2pin, INPUT_PULLUP);
  pinMode(push3pin, INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);
  delay(200);

  
  
  Serial.println(EEPROM.read(addr));
  val2 = EEPROM.read(addr); // read previosuly saved value of push button to start from where it was left previously
  switch (val2) {
    case 1:
      //Serial.println("Set for 1/day");
      push1state = 1;
      push2state = 0;
      push3state = 0;
      pushVal = 1;
      break;
    case 2:
      //Serial.println("Set for 2/day");
      push1state = 0;
      push2state = 1;
      push3state = 0;
      pushVal = 2;
      break;
    case 3:
     //.Serial.println("Set for 3/day");
      push1state = 0;
      push2state = 0;
      push3state = 1;
      pushVal = 3;
      break;
  }
Serial.print("Loading");
}

void loop() {
   push1state = digitalRead(push1pin); // start reading all push button pins
  push2state = digitalRead(push2pin);
  push3state = digitalRead(push3pin);
  stopinState = digitalRead(stopPin);
 timekeep();
  push1(); //call to set once/day
  push2(); //call to set twice/day
  push3(); //call to set thrice/day
  if (pushVal == 1) { // if push button 1 pressed then remind at 8am
    at8am(); //function to start uzzing at 8am
    display.clearDisplay();
    display.setTextSize(2);       //size of the text that will follow
    display.setTextColor(WHITE);  //its color
    display.setCursor(1, 1);     //position from where you want to start writing
    display.print("Reminder 8am"); //text todisplay
    display.display();
    delay(1000);
  }
  else if (pushVal == 2) { // if push button 2 pressed then remind at 8am and 8pm
    at8am();
    at8pm(); //function to start uzzing at 8mm
     display.clearDisplay();
    display.setTextSize(2);       //size of the text that will follow
    display.setTextColor(WHITE);  //its color
    display.setCursor(1, 1);     //position from where you want to start writing
    display.print("Reminder 8am,8pm"); //text todisplay
    display.display();
    delay(1000);
  }
  else if (pushVal == 3) { // if push button 3 pressed then remind at 8am and 8pm
    at8am();
    at2pm(); //function to start uzzing at 8mm
    at8pm();
     display.clearDisplay();
    display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(1,1);
  display.print("Reminder ");
  display.println("8am,2pm,8pm");
  display.display();      // Show initial text
  delay(1000);
  
  }

  stopPins(); // call to stop buzzing
}
// push buttons
void push1() { // function to set reminder once/day
  push1state = digitalRead(push1pin);
  if (push1state == LOW) {
   
    // pushPressed = true;
    EEPROM.update(addr, 1);
    //Serial.print("Push1 Written : "); 
    //Serial.println(EEPROM.read(addr)); // for debugging
    pushVal = 1; //save the state of push button-1
   
  }
}

void push2() { //function to set reminder twice/day
  push2state = digitalRead(push2pin);
  if (push2state == LOW) {
   
    // pushPressed = true;
    EEPROM.update(addr, 2);
    //Serial.print("Push2 Written : "); 
    //Serial.println(EEPROM.read(addr));
    pushVal = 2;
   
  }
}

void push3() { //function to set reminder thrice/day
  push3state = digitalRead(push3pin);
  if (push3state == LOW) {
   
    // pushPressed = true;
    EEPROM.update(addr, 3);
    //Serial.print("Push3 Written : ");
    //Serial.println(EEPROM.read(addr));
    pushVal = 3;
   
  }
}

void stopPins() { //function to stop buzzing when user pushes stop push button
 stopinState = digitalRead(stopPin);
  if (stopinState == LOW) {
    // stopinState = 0;
    // pushPressed = true;
    pushpressed = 1;
    display.clearDisplay();
     display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(1,1);
    display.println("Alarm Stop");
    display.display();
    delay(1000);
  }
}



void startBuzz() { // function to start buzzing when time reaches to defined interval
  // if (pushPressed == false) {
  if (pushpressed == 0) {
   for (int thisNote = 0; thisNote < 8; thisNote++) { // Loop through the notes in the array.
    TimerFreeTone(TONE_PIN, melody[thisNote], duration[thisNote]); // Play melody[thisNote] for duration[thisNote].
    delay(50); // Short delay between notes.
  }
  }
  else if (pushpressed == 1) {
   // Serial.println("pushpressed is true");
    ledState = LOW;
    digitalWrite(ledPin, ledState);
  }
  stopPins();
}

void at8am() { // function to start buzzing at 8am
  Mydatetime = RTC.now(); // Helps in Checking the current time and Date.
  if (int(Mydatetime.hour()) == buzz8amHH) {
    if (int(Mydatetime.minute()) >= buzz8amMM && int(Mydatetime.minute()) <= MM) {
      if (int(Mydatetime.second()) >= buzz8amSS) {
        /////////////////////////////////////////////////////
        startBuzz();
        /////////////////////////////////////////////////////
      }
    }
  }
}
void at2pm() { // function to start buzzing at 2pm
  Mydatetime = RTC.now(); // Helps in Checking the current time and Date.
  if (int(Mydatetime.hour()) == buzz2pmHH ) {
    if (int(Mydatetime.minute()) >= buzz2pmMM && int(Mydatetime.minute()) <= MM ) {
      if (int(Mydatetime.second()) >= buzz2pmSS) {
        ///////////////////////////////////////////////////
        startBuzz();
        //////////////////////////////////////////////////
      }
    }
  }
}

void at8pm() { // function to start buzzing at 8pm
  Mydatetime = RTC.now(); // Helps in Checking the current time and Date.
  if (int(Mydatetime.hour()) == buzz8pmHH) {
    if (int(Mydatetime.minute()) >= buzz8pmMM && int(Mydatetime.minute()) <= MM) {
      if (int(Mydatetime.second()) >= buzz8pmSS) {
        /////////////////////////////////////////////////////
        startBuzz();
        /////////////////////////////////////////////////////
      }
    }
  }
}

void timekeep() {
  Mydatetime = RTC.now(); // Helps in Checking the current time and Date.
  display.clearDisplay();
  display.setTextSize(2);       //size of the text that will follow
  display.setTextColor(WHITE);  //its color
  display.setCursor(0, 0);     //position from where you want to start writing
  display.print(Mydatetime.hour()); //text todisplay
  display.print(":");
  display.print(Mydatetime.minute());
  display.print(":");
  display.print(Mydatetime.second());
  display.display();
  delay(1000);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("WELCOME"); // CHANGE HERE TO YUA LIKING
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrolldiagright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(100);
 
}
void welcome(){
   display.clearDisplay();
   display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.print("Your Health is OUR Concern"); // CHANGE HERE TO YUA LIKING
  display.display();      // Show initial text
  //delay(3000);

   display.startscrollleft(0x00, 0x0F);
  delay(5000);
  display.stopscroll();
  delay(100);
}
