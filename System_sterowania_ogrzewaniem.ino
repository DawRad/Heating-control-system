/* Learn how to use a potentiometer to fade an LED with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/arduino-fade-led
   Dev: Michalis Vasilakis / Date: 25/10/2014                                                   */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Constants:
const int pResistor = A0; //pin A0 to read analog input
const int btnPin = 2;

// //Variables:
int value; //save analog value
boolean room_state;
int btnVal;

void setup(){
  // initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.print("Hello, world!");

  //Input or output?
  pinMode(pResistor, INPUT); //Optional
  pinMode(btnPin,INPUT_PULLUP);
  Serial.begin(9600);

  room_state = false;
}

void loop(){
  value = analogRead(pResistor);          //Read and save analog value from photoresistor
  Serial.println(value);

  btnVal = digitalRead(btnPin);
  if(btnVal==LOW)
  {
    if(room_state)  room_state = false;
    else room_state = true;
  }

  lcd.clear();
  lcd.print("Light lvl: ");
  lcd.print(value);
  lcd.setCursor (0,1); // go to start of 2nd line
  lcd.print("Room: ");
  if(room_state)  lcd.print("occupied");
  else lcd.print("empty");  
  delay(500);
}
