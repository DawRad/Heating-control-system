/* Learn how to use a potentiometer to fade an LED with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/arduino-fade-led
   Dev: Michalis Vasilakis / Date: 25/10/2014                                                   */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Constants:
const int pResistor = A0; //pin A0 to read analog input

// const int ledBtn = 4;
// const int btnPin = 2;

// //Variables:
int value; //save analog value
// boolean ledOn;
// int btnVal;

void setup(){
  // initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.print("Hello, world!");

  //Input or output?
  pinMode(pResistor, INPUT); //Optional
  // pinMode(ledBtn,OUTPUT);
  // pinMode(btnPin,INPUT_PULLUP);
  Serial.begin(9600);

  // ledOn = false;
}

void loop(){
  value = analogRead(pResistor);          //Read and save analog value from photoresistor
  Serial.println(value);

  lcd.clear();
  lcd.print("Light lvl:");
  lcd.setCursor (0,1); // go to start of 2nd line
  lcd.print(value);
  //lcd.print(millis() / 1000);
  delay(500);
  //end of loopcode Robojax code for LCD with I2C
}
