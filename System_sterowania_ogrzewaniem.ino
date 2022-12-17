/* Learn how to use a potentiometer to fade an LED with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/arduino-fade-led
   Dev: Michalis Vasilakis / Date: 25/10/2014                                                   */

//Constants:
const int ledPin = 9;  //pin 9 has PWM funtion
const int potPin = A0; //pin A0 to read analog input

const int ledBtn = 4;
const int btnPin = 2;

//Variables:
int value; //save analog value
boolean ledOn;
int btnVal;

void setup(){
  //Input or output?
  pinMode(ledPin, OUTPUT); 
  pinMode(potPin, INPUT); //Optional
  pinMode(ledBtn,OUTPUT);
  pinMode(btnPin,INPUT_PULLUP);
  Serial.begin(9600);

  ledOn = false;
}

void loop(){
  value = analogRead(potPin);          //Read and save analog value from potentiometer
  Serial.println(value);
  value = map(value, 0, 1023, 0, 255); //Map value 0-1023 to 0-255 (PWM)
  analogWrite(ledPin, value);          //Send PWM value to led

  btnVal = digitalRead(btnPin);
  if(btnVal==LOW)
  {
    digitalWrite(ledBtn,HIGH);
  }
  else
  {
    digitalWrite(ledBtn,LOW);
  }
  delay(100);                          //Small delay  
}
