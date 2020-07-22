/*
 *
 * Control cooker hood and lights project
 *
 */
 
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
//#include <Button.h>

/*
 * Constants definition
 */
#define  PULSE_WIDTH          5      // Value is in uC cycles. Thyristor requires min 2us
#define  GATE_OFFSET_INITIAL  301
#define  GATE_OFFSET_MAX      501    // uC cycles max: 625
#define  GATE_OFFSET_MIN      1
#define  STEP                 25    //step 62.5
#define  COUNTER_MAX          65535  //The counter reaches its MAXimum when it becomes 0xFFFF (decimal 65535).
#define  DEBOUNCE             85

class button {
private:
	byte pin;
	byte state = 0;
	byte reading = LOW;
	long last_DebounceTime;         // the last time the output pin was toggled

public:
	button(byte pin) {
		this->pin = pin;
		pinMode(pin, INPUT);
		last_DebounceTime = 0;
	}

	bool buttonPressed() {
		byte lastState = LOW;
		this->reading = digitalRead(pin);
		if (reading != lastState) {
			lastState = millis(); // reset the debouncing timer
		}
		if ((millis() - last_DebounceTime) > DEBOUNCE) {
			if (reading != state) {
				state = reading;
				if (state == HIGH) {
					return true;
				}
			}
		}
		lastState = reading;
		return false;
	}

	byte getPin() const {
		return pin;
	}

	void setPin(byte pin) {
		this->pin = pin;
	}

	byte getReading() const {
		return reading;
	}

	void setReading(byte reading) {
		this->reading = reading;
	}

	byte getState() const {
		return state;
	}

	void setState(byte state) {
		this->state = state;
	}
};

/*
 * ATMega ports
 */
const byte zerroCrossing = 2;                         // PB2 INT0
const byte control_CookerHoodFan = 11;       // control_CookerHoodFan
button btnOnOff(10);                            // B2
button btnLights(9);                            // B1
button btnMinus(12);                            // B4
button btnPlus(7);                              // D7
button btnMax(8);                               // B0
const byte status_Light = 13;                          // B5
const byte control_CookerHoodLights = 14;             // A0
const byte control_SinkLights = 16;                   // A1
const byte control_BarLights = 17;                    // A2

/*
 * Global variables
 */
volatile float gateOffset = GATE_OFFSET_INITIAL;    // uC cycles
volatile byte onOffState = 0; // initial regulator state when power is connected
byte state_CookerHoodLights = LOW;  // the current state of the output pin
byte state_CookerHoodFan = LOW;     // the current state of the output pin 
byte state_BarLights = LOW;         // the current state of the output pin 
byte state_SinkLights = LOW;        // the current state of the output pin 
byte count_pushes_btnLights = 0;    // counter for the number of button presses
long last_DebounceTime = 0;         // the last time the output pin was toggled
unsigned long startingTime = 0;

/*
 * Interrupt, zero cross detect
 */
void zeroCrossInterrupt() {
	TCCR1B = 0x04;  //start timer with divide by 256 input
	TCNT1 = 0;      //reset timer - count from zero
}

ISR(TIMER1_COMPA_vect) { //comparator match
  digitalWrite(control_CookerHoodFan, HIGH); //set triac gate to high
	TCNT1 = COUNTER_MAX - PULSE_WIDTH;      //trigger pulse width
}

ISR(TIMER1_OVF_vect) {        //timer1 overflow
	digitalWrite(control_CookerHoodFan, LOW); //turn off triac gate
	TCCR1B = 0x00;              //disable timer stop unintended triggers
}

/*
 * Function that reads buttons pressed by user
 */
void readButtons() {

	/* LIGHTS */
	if (btnLights.buttonPressed()) {
		if (count_pushes_btnLights == 0) {
			startingTime = millis();
		}
		count_pushes_btnLights++; // count button pushes if 'timeout' is not reached
	}
	handleLights();

	/* OnOff */
	if (btnOnOff.buttonPressed()) {
		onOffState = !onOffState;
		gateOffset = GATE_OFFSET_INITIAL;
		Serial.print("Power state = ");
		Serial.print(onOffState);
		Serial.print("; Gate offset = ");
		Serial.print(gateOffset);
		Serial.print("\n");
    Wire.beginTransmission(2);
    Wire.write("Power state = ");
    Wire.write(onOffState);
    Wire.endTransmission();
	}

	if (onOffState == 1) { // OnOff check

		/* MINUS */
		if (btnMinus.buttonPressed()) {
			if (gateOffset < GATE_OFFSET_MAX) {
				gateOffset = gateOffset + STEP;
			}
			Serial.print("- pressed; Gate offset = ");
			Serial.print(gateOffset);
			Serial.print("\n");
		}

		/* PLUS */
		if (btnPlus.buttonPressed()) {
			if (gateOffset > GATE_OFFSET_MIN) {
				gateOffset = gateOffset - STEP;
			}
			Serial.print("+ pressed; Gate offset = ");
			Serial.print(gateOffset);
			Serial.print("\n");
		}

		/* MAX */
		if (btnMax.buttonPressed()) {
			if (gateOffset != GATE_OFFSET_MIN) {
				gateOffset = GATE_OFFSET_MIN;
			}
			Serial.print("Max Power. Gate offset = ");
			Serial.print(gateOffset);
			Serial.print("\n");
		}
	} // end of OnOff check
} // end of readButtons

void pushButtonHandler(byte * state, byte* count, byte pin) {
	*state = !*state;
	digitalWrite(pin, *state);
	Serial.print("pin");
	Serial.print(pin);
	Serial.print(" ");
	Serial.print(*state);
	Serial.print(" button pressed ");
	Serial.print(*count);
	Serial.print(" times");
	Serial.print("\n");
	*count = 0;
}

/*
 * Handle lights
 */
void handleLights() {
	const unsigned long timeout = 1000;

	if (millis() - startingTime > timeout) {

		switch (count_pushes_btnLights) {
		default:
			count_pushes_btnLights = 0;
			break;

		case 1: // count_pushes_btnLights = 1
			pushButtonHandler(&state_CookerHoodLights, &count_pushes_btnLights,
					control_CookerHoodLights);
      Wire.beginTransmission(2);
      Wire.write("Cooker Hood Lights = ");
      Wire.write(state_CookerHoodLights);
      Wire.endTransmission();
			break;

		case 2: // count_pushes_btnLights = 2
			pushButtonHandler(&state_SinkLights, &count_pushes_btnLights,
					control_SinkLights);
      Wire.beginTransmission(2);
      Wire.write("Sink Lights = ");
      Wire.write(state_SinkLights);
      Wire.endTransmission();
			break;

		case 3: // count_pushes_btnLights = 3
			pushButtonHandler(&state_BarLights, &count_pushes_btnLights,
					control_BarLights);
      Wire.beginTransmission(2);
      Wire.write("Bar Lights = ");
      Wire.write(state_BarLights);
      Wire.endTransmission();
			break;

		} // end of switch
	}
} // end of handleLights

void setup() {
  Wire.begin(1);                // join i2c bus with address #1
	Serial.begin(115200);
	Serial.println("Initializing ...\n");
	Serial.println(" - Configuring pins");
	pinMode(zerroCrossing, INPUT_PULLUP);
  pinMode(status_Light, OUTPUT);
	pinMode(control_CookerHoodFan, OUTPUT);
  pinMode(control_CookerHoodLights, OUTPUT);
	pinMode(control_SinkLights, OUTPUT);
	pinMode(control_BarLights, OUTPUT);

// set initial Lights state to OFF
	Serial.println(" - Put Lights to Off");
	digitalWrite(control_CookerHoodLights, state_CookerHoodLights);
	digitalWrite(control_BarLights, state_BarLights);
	digitalWrite(control_SinkLights, state_SinkLights);
	Serial.println(" - Put Fan to Off");
	digitalWrite(control_CookerHoodFan, state_CookerHoodFan);

// set up Timer1
//(see ATMEGA 328 data sheet pg 134 for more details)
	Serial.println(" - Setting up timer");
	OCR1A = 100;      //initialize the comparator
	TIMSK1 = 0x03;    //enable comparator A and overflow interrupts
	TCCR1A = 0x00;    //timer control registers set for
	TCCR1B = 0x00;    //normal operation, timer disabled

  //attachInterrupt(digitalPinToInterrupt(zerroCrossing), zeroCrossInterrupt, RISING);

	Serial.print(" - gateOffset = ");
	Serial.print(gateOffset);
	Serial.print("\n");
	Serial.println("\nInitialization completed");
	Serial.println(
			"\nRunning operation mode ...\n============================\n");
} // end of setup

void loop() {
	readButtons();
	if (onOffState == 0) {
		digitalWrite(control_CookerHoodFan, LOW);
		detachInterrupt(digitalPinToInterrupt(zerroCrossing));
    digitalWrite(status_Light, LOW);
	} else if (onOffState == 1) {
		attachInterrupt(digitalPinToInterrupt(zerroCrossing), zeroCrossInterrupt, RISING);
    digitalWrite(status_Light, HIGH);
    Serial.println(gateOffset);
	}
	OCR1A = gateOffset;
 
} //end of loop
