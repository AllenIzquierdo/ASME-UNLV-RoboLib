#ifndef POLOLUG2_H
#define POLOLUG2_H
#include <Arduino.h>
#include <Motor.h>

class PololuG2: public Motor {
	public:
		PololuG2(const byte enable_pin, const byte pwm_pin, const byte dir_pin);
		
		// Setters and Getters
		void setPower(const float power);
		byte output;
	protected:
	private:
		byte enable_pin;
		byte pwm_pin;
		byte dir_pin;
};

#endif
