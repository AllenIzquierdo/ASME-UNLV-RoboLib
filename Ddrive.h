#ifndef DDRIVE_H
#define DDRIVE_H
#include <Arduino.h>
#include <Motor.h>
#include <Vector_temp.h>
#include <math.h>


/** \brief DriveBase for the Defense Drive Chassis.
 *
 * Defense Drive chassis for Outlaw drive chassis.
 * The alogrithem behind driving the chassis is unorthodox, require free body diagram analysis of the drive chassis. Due to this, refer to Allen for detailed explanation on drive chassis algorithem.
 */

class Ddrive{
	public:
		Ddrive(Motor&, Motor&, Motor&);
		void drive(const float dir, const float thrust, const float turnfactor);
		void reverseMotor(const unsigned char motor, const bool value);
		bool getReverseValue(const unsigned char motor);
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
