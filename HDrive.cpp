#include <HDrive.h>

HDrive::HDrive(const Motor& leftMotor, const Motor& rightMotor, const Motor& middleMotor)
{
	this->leftMotor = &leftMotor;
	this->rightMotor = &rightMotor;
	this->middleMotor = &middleMotor;
}

void HDrive::drive(float left, float right, float middle)
{
	if(reverseLeft)
		leftMotor->setPower(-left);
	else
		leftMotor->setPower(left);

	if(reverseRight)
		rightMotor->setPower(-right);
	else
		rightMotor->setPower(right);

	if(reverseMiddle)
		middleMotor->setPower(-middle);
	else
		middleMotor->setPower(middle);
}

void HDrive::reverseMotor(HDRIVE_MOTOR motor, bool value)
{
	switch(motor)
	{
		case HDRIVE_MOTOR::LEFT:
			reverseLeft = value;
			break;
		case HDRIVE_MOTOR::RIGHT:
			reverseRight = value;
			break;
		case HDRIVE_MOTOR::MIDDLE:
			reverseMiddle = value;
			break;
		default:
			break;
	}
}
