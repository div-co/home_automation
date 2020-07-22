/*
 * Button.h
 *
 *  Created on: Jun 30, 2016
 *      Author: I033821
 *
 *  Members `reading` and `state` needs initialization in main program
 *  # DEBOUNCE definition required
 *
 *
 */

#ifndef BUTTON_H_
#define BUTTON_H_

class button {
	private:
		byte pin;
		byte state;
		byte reading;
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
				last_DebounceTime = millis(); // reset the debouncing timer
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

#endif /* BUTTON_H_ */
