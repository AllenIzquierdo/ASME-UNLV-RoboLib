#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>


/** \brief This is the Abstract Class used as the base class for speed controllers.
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
