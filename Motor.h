#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>


/** \brief Abstract Class used as base class for speed controllers.
 */
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
