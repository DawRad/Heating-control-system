/* Learn how to use a potentiometer to fade an LED with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/arduino-fade-led
   Dev: Michalis Vasilakis / Date: 25/10/2014                                                   */

// #include <string.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Constants:
const int pResistor = A0; //pin A0 to read analog input
const int btnPin = 2;
const int tempPin = A2;

// //Variables:
int value; //save analog value
boolean room_state;
int btnVal;
float actTemp;
float tempAvg;
String strRoomState;

void setup(){
  // initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	// lcd.backlight();
	lcd.print("Hello, world!");

  //Input or output?
  pinMode(pResistor, INPUT); //Optional
  pinMode(tempPin, INPUT);
  pinMode(btnPin,INPUT_PULLUP);
  Serial.begin(9600);

  room_state = false;
  actTemp = 0;
  tempAvg = 0;
  strRoomState = "empty";
}

void loop()
{
  tempAvg = 0;
  for (int i = 0; i < 60; i++)
  {
    actTemp = ((analogRead(tempPin) * 5.0) / 1024) * 10;
    tempAvg += actTemp;
    delay(50);
  }
  tempAvg = tempAvg / 60;

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
  if(room_state)  strRoomState = "occupied";
  else strRoomState = "empty";  
  lcd.print(strRoomState);
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Room: ");
  lcd.print(strRoomState);
  lcd.setCursor(0, 1);
  lcd.print("Temp.: ");
  lcd.print(tempAvg);    
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp.: ");
  lcd.print(tempAvg);
  delay(1500);
}
