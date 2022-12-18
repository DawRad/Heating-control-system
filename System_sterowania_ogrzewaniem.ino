/* Learn how to use a potentiometer to fade an LED with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/arduino-fade-led
   Dev: Michalis Vasilakis / Date: 25/10/2014                                                   */

//Constants:
const int pResistor = A0; //pin A0 to read analog input

// const int ledBtn = 4;
// const int btnPin = 2;

// //Variables:
int value; //save analog value
// boolean ledOn;
// int btnVal;

void setup(){
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
  
  delay(100);                          //Small delay  
}
