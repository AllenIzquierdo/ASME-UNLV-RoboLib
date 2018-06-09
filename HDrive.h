#ifndef HDRIVE_H
#define HDRIVE_H

#include <Motor.h>

class HDrive {
	public:
		enum HDRIVE_MOTOR{LEFT, RIGHT, MIDDLE};

		HDrive(const Motor&, const Motor&, const Motor&);
		void drive(float, float, float);
		void reverseMotor(HDRIVE_MOTOR, bool);

	private:
		bool reverseLeft;
		bool reverseRight;
		bool reverseMiddle;
		Motor* leftMotor;
		Motor* rightMotor;
		Motor* middleMotor;
};
#endif
