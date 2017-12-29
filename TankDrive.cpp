#include <TankDrive.h>

TankDrive::TankDrive(const Motor* leftMotor1, const Motor* leftMotor2, const Motor* rightMotor1, const Motor* rightMotor2)
{
	/*
	this->leftMotor1 = &leftMotor1;
	this->leftMotor2 = &leftMotor2;
	this->rightMotor1 = &rightMotor1;
	this->rightMotor2 = &rightMotor2;
	*/
	this->leftMotor1 = leftMotor1;
	this->leftMotor2 = leftMotor2;
	this->rightMotor1 = rightMotor1;
	this->rightMotor2 = rightMotor2;
}

void TankDrive::drive(PS2X ps2x)
{

	this->drive(ps2x.JoyStick(PSS_LY), ps2x.JoyStick(PSS_RY));
}

void TankDrive::drive(float left, float right)
{
	if(reverseLeft)
	{
		this->leftMotor1->setPower(-left);
		this->leftMotor2->setPower(-left);
	} else {
		this->leftMotor1->setPower(left);
		this->leftMotor2->setPower(left);
	}

	if(reverseRight)
	{
		this->rightMotor1->setPower(-right);
		this->rightMotor2->setPower(-right);
	} else {
		this->rightMotor1->setPower(right);
		this->rightMotor2->setPower(right);
	}

}

void TankDrive::reverseLeftMotors(bool enable)
{
	this->reverseLeft = enable;
}

void TankDrive::reverseRightMotors(bool enable)
{
	this->reverseRight = enable;
}

