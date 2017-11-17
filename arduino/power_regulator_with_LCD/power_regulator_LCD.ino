#include <LiquidCrystal.h>

/*
 * Constants definition
 */
#define  PULSE_WIDTH        5      // Value is in uC cycles. Thyristor requires min 2us
#define  GATE_OFFSET_MAX    625    // uC cycles
#define  GATE_OFFSET_MIN    0
#define  STEP               6.25
#define  COUNTER_MAX        65535  //The counter reaches its MAXimum when it becomes 0xFFFF (decimal 65535).
#define  DELAY_CALIBRATION  62.5                //used in handleDisplay()
#define  ANGLE_CALIBRATION  3.472222222222222;  //used in handleDisplay()
#define  DEBOUNCE           80

/*
 * Pinout definition
 */
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // LCD pins: RS,E,D4,D5,D6,D7

byte zerroCrossing = 2;          // PB2 INT0
volatile byte gateVS1 = 3;       // PB3
volatile byte gateVS2 = 4;       // PB4
byte  btnMinus = 14;             // A0
byte  btnPlus = 15;              // A1
byte  btnFunc = 16;              // A2
byte  btnOnOff = 17;             // A3
byte  lcdBackLight = 18;         // A4
float loadMonitor = 19;          // A5 analog input

/*
 * Global variables
 */
volatile float gateOffset = GATE_OFFSET_MAX;    // uC cycles
volatile byte onOffState = 0;                   // initial regulator state when power is connected
byte btnMinusState = 0;
byte btnPlusState = 0;
byte btnFuncState = 0;
byte btnMenu = 0;                    // Menu selection
byte btnOnOffState = 0;
int  last_btnMinusState = LOW;
int  last_btnPlusState = LOW;
int  last_btnFuncState = LOW;
int  last_btnOnOffState = LOW;
long last_DebounceTime = 0;          // the last time the output pin was toggled
int  last_gateOffset;                // Last user defined gateOffset value

/*
 * Interrupt, zero cross detect
 */
void zeroCrossInterrupt()
{
  TCCR1B = 0x04;  //start timer with divide by 256 input
  TCNT1 = 0;      //reset timer - count from zero
}

ISR(TIMER1_COMPA_vect) { //comparator match
  if (onOffState == 1) {
    if (digitalRead(zerroCrossing) == 1) {
      digitalWrite(gateVS1, HIGH); //set triac gate to high
    } else if (digitalRead(zerroCrossing) == 0) {
      digitalWrite(gateVS2, HIGH); //set triac gate to high
    }
  } else {
    digitalWrite(gateVS1, LOW); //turn off triac gate
    digitalWrite(gateVS2, LOW); //turn off triac gate
  }
  TCNT1 = COUNTER_MAX - PULSE_WIDTH;      //trigger pulse width
}

ISR(TIMER1_OVF_vect) {        //timer1 overflow
  digitalWrite(gateVS1, LOW); //turn off triac gate
  digitalWrite(gateVS2, LOW); //turn off triac gate
  TCCR1B = 0x00;              //disable timer stopd unintended triggers
}

/*
 * Handle regulator behaviour on overload
 */
void handleOverload()
{
  noInterrupts();
  digitalWrite(gateVS1, LOW);
  digitalWrite(gateVS2, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OVERLOAD");
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("PLEASE RESTART !");
  digitalWrite(lcdBackLight, LOW);
  delay(10000);
  digitalWrite(lcdBackLight, HIGH);
  delay(35000);
} // end of handleOverload

/*
 * Function that reads buttons pressed by user
 */
void readButtons()
{
  byte reading_btnOnOff;
  byte reading_btnFunc;
  byte reading_btnPlus;
  byte reading_btnMinus;

  reading_btnOnOff = digitalRead(btnOnOff);
  reading_btnFunc = digitalRead(btnFunc);
  reading_btnPlus = digitalRead(btnPlus);
  reading_btnMinus = digitalRead(btnMinus);

  ////////////////////////////////////////////// OnOff /////////////////////////////////////////////////////
  if (reading_btnOnOff != last_btnOnOffState) {
    last_btnOnOffState = millis(); // reset the debouncing timer
  }
  if ((millis() - last_DebounceTime) > DEBOUNCE) {
    if (reading_btnOnOff != btnOnOffState) {
      btnOnOffState = reading_btnOnOff;
      if (btnOnOffState == HIGH) {
        onOffState = !onOffState;
        handleDisplay();
      }
    }
  }
  last_btnOnOffState = reading_btnOnOff;
  ////////////////////////////////////////////// FUNC /////////////////////////////////////////////////////
  if (reading_btnFunc != last_btnFuncState) {
    last_btnFuncState = millis(); // reset the debouncing timer
  }
  if ((millis() - last_DebounceTime) > DEBOUNCE) {
    if (reading_btnFunc != btnFuncState) {
      btnFuncState = reading_btnFunc;
      if (btnFuncState == HIGH) {
        if (btnMenu < 2) {
          btnMenu++;
          handleDisplay();
        } else {
          btnMenu = 0;
          handleDisplay();
        }
      }
    }
  }
  last_btnFuncState = reading_btnFunc;
  ////////////////////////////////////////////// PLUS /////////////////////////////////////////////////////
  if (reading_btnPlus != last_btnPlusState) {
    last_btnPlusState = millis(); // reset the debouncing timer
  }
  if ((millis() - last_DebounceTime) > DEBOUNCE) {
    if (reading_btnPlus != btnPlusState) {
      btnPlusState = reading_btnPlus;
      if (btnPlusState == HIGH) {
        if (gateOffset > GATE_OFFSET_MIN) {
          gateOffset = gateOffset - STEP;
          handleDisplay();
        }
      }
    }
  }
  last_btnPlusState = reading_btnPlus;
  ////////////////////////////////////////////// MINUS /////////////////////////////////////////////////////
  if (reading_btnMinus != last_btnMinusState) {
    last_btnMinusState = millis(); // reset the debouncing timer
  }
  if ((millis() - last_DebounceTime) > DEBOUNCE) {
    if (reading_btnMinus != btnMinusState) {
      btnMinusState = reading_btnMinus;
      if (btnMinusState == HIGH) {
        if (gateOffset < GATE_OFFSET_MAX) {
          gateOffset = gateOffset + STEP;
          handleDisplay();
        }
      }
    }
  }
  last_btnMinusState = reading_btnMinus;

} // end of readButtons

/*
 * Handle disply information
 */
void handleDisplay()
{
  float displayDelay = gateOffset / DELAY_CALIBRATION;
  float displayAngle = gateOffset / ANGLE_CALIBRATION;

  lcd.clear();
  switch (btnMenu) {

    case 1: // display pulse delay
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pulse delay");
      lcd.setCursor(0, 1);
      lcd.print(displayDelay, 1);
      lcd.print("ms");
      break;

    case 2: // display alpha
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Angle");
      lcd.setCursor(0, 1);
      lcd.print(displayAngle, 0);
      lcd.print((char)223);
      break;

    default: // normal operation
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("D= ");
      lcd.print(displayDelay, 1);
      lcd.print("ms");
      lcd.setCursor(0, 1);
      lcd.print("A= ");
      lcd.print(displayAngle, 0);
      lcd.print((char)223);
      lcd.setCursor(0, 1);
      break;

  } // end of switch

  if (onOffState == 1) {
    lcd.setCursor(14, 1);
    lcd.print("ON");
  } else if (onOffState == 0) {
    lcd.setCursor(13, 1);
    lcd.print("OFF");
  }

} // end of handleDisplay

void setup()
{
  lcd.begin(16, 2);
  digitalWrite(lcdBackLight, HIGH);
  handleDisplay();

  pinMode(zerroCrossing, INPUT);
  pinMode(gateVS1, OUTPUT);
  pinMode(gateVS2, OUTPUT);
  pinMode(lcdBackLight, OUTPUT);
  pinMode(btnMinus, INPUT);
  pinMode(btnPlus, INPUT);
  pinMode(btnFunc, INPUT);
  pinMode(btnOnOff, INPUT);
  pinMode(loadMonitor, INPUT);

  // set up Timer1
  //(see ATMEGA 328 data sheet pg 134 for more details)
  OCR1A = 100;      //initialize the comparator
  TIMSK1 = 0x03;    //enable comparator A and overflow interrupts
  TCCR1A = 0x00;    //timer control registers set for
  TCCR1B = 0x00;    //normal operation, timer disabled
} // end of setup

void loop()
{
  float readLoadCurrent = analogRead(loadMonitor); // read the A5 voltage

  if (readLoadCurrent < 1000.0) { // compare analog pin voltage
    readButtons();
    if (onOffState == 0) {
      digitalWrite(gateVS1, LOW);
      digitalWrite(gateVS2, LOW);
      detachInterrupt(0);
    } else if (onOffState == 1) {
      attachInterrupt(0, zeroCrossInterrupt, CHANGE);
    }
    OCR1A = gateOffset;
  } else {
    handleOverload(); // stop service routine and call function to handle overload
  }

} //end of loop
