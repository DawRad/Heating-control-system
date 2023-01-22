/* Learn how to use a potentiometer to fade an LED with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/arduino-fade-led
   Dev: Michalis Vasilakis / Date: 25/10/2014                                                   */

// #include <string.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// LCD special characters
uint8_t lvl_1[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F};
uint8_t lvl_2[8] = {0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F};
uint8_t lvl_3[8] = {0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
uint8_t lvl_full[8] = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};

// specific enum type for the system
typedef enum
{
  SYS_DOWN = 0,
  ROOM_STATE_BUTTON,
  OFF_BUTTON,  
  TWO_BUTTONS,
  UNKNOWN_STATE = -1
} e_sys_state;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Constants:
const int pResistor = A0; //pin A0 to read analog input
const int btnPin = 2;
const int offBtnPin = 4;
// const int tempPin = A2;

const int lightThresh = 5;  // threshold of light measurment
const float tempThresh = 25.0;  

const int P_RESIST_MAX_VAL = 1017;
const int P_RESIST_MIN_VAL = 0;

const int HEAT_LVL_MAX = 15;
const int HEAT_LVL_MIN = 1;

// //Variables:
int value; //save analog value
boolean room_state;
boolean offBtnState;
int btnVal;
int offBtnVal;
float actTemp;
int tempCnt;
String strRoomState;
e_sys_state actButtonState;

void setup(){
  // initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	// lcd.backlight();
	lcd.print("Hello, world!");

  //Input or output?
  pinMode(pResistor, INPUT); //Optional
  // pinMode(tempPin, INPUT);
  pinMode(btnPin,INPUT_PULLUP);
  pinMode(offBtnPin,INPUT_PULLUP);
  Serial.begin(9600);

  room_state = false;
  offBtnState = false;
  actTemp = 0;
  tempCnt = 0;
  strRoomState = "empty";
  actButtonState = UNKNOWN_STATE;

  randomSeed(analogRead(0));
  float tempInt = random(18,22);
  float tempDec = random(9) / 10.0;
  actTemp = tempInt + tempDec;

  // creating characters
  lcd.createChar(1, lvl_1);
  lcd.createChar(2, lvl_2);
  lcd.createChar(3, lvl_3);
  lcd.createChar(4, lvl_full);
}

void loop()
{
  if(tempCnt == 5)
  {
    float tempInt = random(18,22);
    float tempDec = random(9) / 10.0;
    actTemp = tempInt + tempDec;
    tempCnt = 0;
  }
  tempCnt++;

  value = analogRead(pResistor);          //Read and save analog value from photoresistor
  Serial.println(value);
  // normalize value from photoresistance
  value = constrain(value, P_RESIST_MIN_VAL, P_RESIST_MAX_VAL);
  value = map(value, P_RESIST_MIN_VAL, P_RESIST_MAX_VAL, HEAT_LVL_MIN, HEAT_LVL_MAX);
  
  btnVal = digitalRead(btnPin);
  if(btnVal==LOW)
  {
    if(room_state)  room_state = false;
    else room_state = true;
  }

  offBtnVal = digitalRead(offBtnPin);
  if(offBtnVal==LOW)
  {
    if(offBtnState) offBtnState = false;
    else offBtnState = true;
  }

  lcd.clear();
  lcd.print("Light lvl: ");
  lcd.print(value);
  lcd.setCursor (0,1); // go to start of 2nd line
  lcd.print("Room: ");
  if(room_state)  strRoomState = "occupied";
  else strRoomState = "empty";  
  lcd.print(strRoomState);

  actButtonState = checkButtons(50,20,actButtonState);
  // delay(1000);

  // *****=====-----> condition after 1st button check
  if(actButtonState == TWO_BUTTONS)
  {
    lcd.clear();
    lcd.print("Saving state...");
    lcd.setCursor (0,1); // go to start of 2nd line
    lcd.print("Good or not?");

    boolean loop_end = false;
    while(!loop_end)
    {
      delay(50);
      btnVal = digitalRead(btnPin);      
      offBtnVal = digitalRead(offBtnPin);
      if(btnVal==LOW)
      {        
        loop_end = true;
        lcd.clear();
        lcd.print("Saved as good");
        delay(1000);
      }      
      else if(offBtnVal==LOW)
      {
        loop_end = true;
        lcd.clear();
        lcd.print("Saved as bad");
        delay(1000);
      }
    }
    actButtonState = UNKNOWN_STATE;
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Room: ");
    lcd.print(strRoomState);
    lcd.setCursor(0, 1);
    lcd.print("Temp.: ");
    lcd.print(actTemp);
    lcd.print(" *C"); 
    
    actButtonState = checkButtons(50,20,actButtonState);  
    // delay(1000);
    
    // *****=====-----> condition after 2nd button check
    if(actButtonState == TWO_BUTTONS)
    {
      lcd.clear();
      lcd.print("Saving state...");
      lcd.setCursor (0,1); // go to start of 2nd line
      lcd.print("Good or not?");

      boolean loop_end = false;
      while(!loop_end)
      {
        delay(50);
        btnVal = digitalRead(btnPin);      
        offBtnVal = digitalRead(offBtnPin);
        if(btnVal==LOW)
        {        
          loop_end = true;
          lcd.clear();
          lcd.print("Saved as good");
          delay(1000);
        }      
        else if(offBtnVal==LOW)
        {
          loop_end = true;
          lcd.clear();
          lcd.print("Saved as bad");
          delay(1000);
        }
      }
      actButtonState = UNKNOWN_STATE;
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp.: ");
      lcd.print(actTemp);
      lcd.print(" *C");
        
      actButtonState = checkButtons(50,20,actButtonState);  
      // delay(1000);

      // *****=====-----> condition after 3rd button check
      if(actButtonState == TWO_BUTTONS)
      {
        lcd.clear();
        lcd.print("Saving state...");
        lcd.setCursor (0,1); // go to start of 2nd line
        lcd.print("Good or not?");

        boolean loop_end = false;
        while(!loop_end)
        {
          delay(50);
          btnVal = digitalRead(btnPin);      
          offBtnVal = digitalRead(offBtnPin);
          if(btnVal==LOW)
          {        
            loop_end = true;
            lcd.clear();
            lcd.print("Saved as good");
            delay(1000);
          }      
          else if(offBtnVal==LOW)
          {
            loop_end = true;
            lcd.clear();
            lcd.print("Saved as bad");
            delay(1000);
          }
        }
        actButtonState = UNKNOWN_STATE;
      }
      else
      {
        lcd.clear();
      
        lcd.setCursor(0, 0);
        if(offBtnState)
        {
          lcd.print("--> Light: Off");
          lcd.setCursor(0, 1);
          lcd.print("--> Heat.: ");
          displayLevels(lcd, value);
        }
        else 
        {
            if(value <= lightThresh)
          {
            if(room_state)
            {      
              lcd.print("--> Light: On");
            }
            else
            {    
              lcd.print("--> Light: Off");
            }
          }
          else
          {
            lcd.print("--> Light: Off");
            lcd.setCursor(0, 1);
            lcd.print("--> Heat.: ");
            displayLevels(lcd, value);
          }
        }
          
        actButtonState = checkButtons(50,20,actButtonState);
      } 
    }                
  }
  
  // *****=====-----> condition after 4th button check
  if(actButtonState == TWO_BUTTONS)
  {
    lcd.clear();
    lcd.print("Saving state...");
    lcd.setCursor (0,1); // go to start of 2nd line
    lcd.print("Good or not?");

    boolean loop_end = false;
    while(!loop_end)
    {
      delay(50);
      btnVal = digitalRead(btnPin);      
      offBtnVal = digitalRead(offBtnPin);
      if(btnVal==LOW)
      {        
        loop_end = true;
        lcd.clear();
        lcd.print("Saved as good");
        delay(1000);
      }      
      else if(offBtnVal==LOW)
      {
        loop_end = true;
        lcd.clear();
        lcd.print("Saved as bad");
        delay(1000);
      }
    }
  }

  // delay(1000);
  actButtonState = UNKNOWN_STATE;
}

void displayLevels(LiquidCrystal_I2C lcd, int lvl_val)
{
  lvl_val = HEAT_LVL_MAX - lvl_val + HEAT_LVL_MIN;  
  int full_lvls_cnt = lvl_val / 4;
  int fractial_lvl_cnt = lvl_val % 4;
  
  for(int i = 0; i < full_lvls_cnt; i++)
  {
    lcd.write(4);
  } 

  if(fractial_lvl_cnt != 0)
  {
    lcd.write(fractial_lvl_cnt);
  }
}

/** \brief Checks whether states of buttons was changed.
 *
 * \param interval - value of a pause between iterations
 * \param iters - number of iterations
 * \param act_state - actual state of the button check procedure
 */ 
e_sys_state checkButtons(int interval, int iters, e_sys_state act_state)
{
  e_sys_state res = UNKNOWN_STATE;
  for(int i = 0; i < iters; ++i)
  {
    btnVal = digitalRead(btnPin);
    offBtnVal = digitalRead(offBtnPin);
    if(btnVal == LOW && offBtnVal == LOW)
    {
      res = TWO_BUTTONS;
    }
    else
    {
      if(btnVal==LOW && res != TWO_BUTTONS)
      {
        res = ROOM_STATE_BUTTON;
      }
      
      if(offBtnVal==LOW && res != TWO_BUTTONS)
      {
        res = OFF_BUTTON;
      }
    }
    // Serial.println("btnVal = ");
    // Serial.println(bt)
    delay(interval);
  }
  
  if(res==ROOM_STATE_BUTTON && act_state == UNKNOWN_STATE)
  {
    if(room_state)  room_state = false;
    else room_state = true;
  }
  
  if(res==OFF_BUTTON)
  {
    if(offBtnState) offBtnState = false;
    else offBtnState = true;
  }  

  return res;
}