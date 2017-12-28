#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>


class Motor {
	public:
		Motor(const unsigned char id);

		// Setters and Getters
		virtual void setPower(const float power);
		float getPower();

	protected:

	private:
		float power;
		unsigned char id;
};

#endif
