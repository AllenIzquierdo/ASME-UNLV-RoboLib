#include <TankDrive.h>

/** \brief Constructs the TankDrive with 4 motors.
 * 
 * Arguments must be speed controllers derived from abstract Motor class.
 * \param leftMotor1 Speed controller for left motor. 
 * \param leftMotor2 Speed controller for left motor.
 * \param rightMotor1 Speed controller for right motor.
 * \param rightMotor1 Speed controller for right motor.
 */
TankDrive::TankDrive(const Motor* leftMotor1, const Motor* leftMotor2, const Motor* rightMotor1, const Motor* rightMotor2)
{
	this->leftMotor1 = &leftMotor1;
	this->leftMotor2 = &leftMotor2;
	this->rightMotor1 = &rightMotor1;
	this->rightMotor2 = &rightMotor2;
}

/** \brief Drives TankDrive with ps2 controller.
 *
 * Updates TankDrive once, using ps2x controller. Usually called repeatedly in loop().
 * \param ps2x ps2 controller used to controll the TankDrive.
 */
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

/** \brief Reverses leftMotors' foward direction.
 *
 * To be used when Allen is too lazy to rewire the robot.
 * True: driving tankdrive left motors foward will actually drive left motors in reverse, and vice versa.
 * False: Normal behavior.
 * \param enable Reverse left motors foward direction.
 */
void TankDrive::reverseLeftMotors(bool enable)
{
	this->reverseLeft = enable;
}

/** \brief See reverseLeftMotors()
 */
void TankDrive::reverseRightMotors(bool enable)
{
	this->reverseRight = enable;
}

