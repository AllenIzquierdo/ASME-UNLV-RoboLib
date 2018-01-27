#ifndef DDRIVE_H
#define DDRIVE_H
#include <Arduino.h>
#include <Motor.h>
#include <Vector_temp.h>
#include <math.h>


class Ddrive{
	public:
		Ddrive(Motor&, Motor&, Motor&);
		void drive(const float dir, const float thrust, const float turnfactor);
		void reverseMotor(const unsigned char motor, const bool value);
		void smartConstrain(float &val1, float &val2);
	protected:
	private:
		Motor* leftmotor;
		Motor* rightmotor;
		Motor* middlemotor;
		bool reverseLeftMotor;
		bool reverseRightMotor;
		bool reverseMiddleMotor;
};

#endif
