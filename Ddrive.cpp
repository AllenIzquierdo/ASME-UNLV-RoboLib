#include <Ddrive.h>

/** \brief Constructor for Ddrive class
 *
 * Refere to Outlaw documentation for use.
 *
 * For the purpose of reversing motor, left motor, middle motor, and right motor are motor 1, 3, and 2 respectively.
 * \param leftmotor 	- back left motor
 * \param middle motor 	- back middle motor
 * \param &rightmotor 	- back right motor
 */
Ddrive::Ddrive(Motor &leftmotor, Motor &rightmotor, Motor &middlemotor)
{
	this->leftmotor = &leftmotor;
	this->rightmotor = &rightmotor;
	this->middlemotor = &middlemotor;
}
 

/** \brief This is the defense drive's drive function, which controlls the chassis by direction, thrust, and turnfactor values.
 * \param dir     - The direction we drive towards, this is measured in radians.
 * \param thrust  - This is how much power is to be given to the motors. The input value range is -1 to 1. If the value is zero, the motors
 won't be given any power. The thrust is calculated with vector math.
 * \param turn    - This is the turning factor. Input range is -1 to 1, if zero, no turning will be done.
 */
void Ddrive::drive(const float dir, const float thrust, const float turnfactor)
{
	float Lthrust = Vec2::dot(Vec2(.707, .707), Vec2(cos(dir), sin(dir)));
	float Rthrust = Vec2::dot(Vec2(-.707, .707), Vec2(cos(dir), sin(dir)));
	float Mthrust = cos(dir);

	float Labs = fabs(Lthrust);
	float Rabs = fabs(Rthrust);
	float Mabs = fabs(Mthrust);

	if(Labs > Rabs && Labs > Mabs)
	{
		Lthrust = Lthrust/Labs;
		Rthrust = Rthrust/Labs;
		Mthrust = Mthrust/Labs;
	} else if(Rabs > Labs && Rabs > Mabs)
	{
		Lthrust = Lthrust/Rabs;
		Rthrust = Rthrust/Rabs;
		Mthrust = Mthrust/Rabs;
	} else if(Mabs > Labs && Mabs > Rabs)
	{
		Lthrust = Lthrust/Mabs;
		Rthrust = Rthrust/Mabs;
		Mthrust = Mthrust/Mabs;
	}

	Lthrust = Lthrust * thrust + turnfactor;
	Rthrust = Rthrust * thrust - turnfactor;
	Mthrust = Mthrust * thrust - turnfactor;

	Mthrust = constrain(Mthrust, -1, 1);
	smartConstrain(Lthrust, Rthrust);

	if(reverseLeftMotor)
	{
		leftmotor->setPower(-Lthrust);
	} else {
		leftmotor->setPower(Lthrust);
	}

	if(reverseRightMotor)
	{
		rightmotor->setPower(-Rthrust);
	} else {
		rightmotor->setPower(Rthrust);
	}

	if(reverseMiddleMotor)
	{
		middlemotor->setPower(-Mthrust);
	} else {
		middlemotor->setPower(Mthrust);
	}

}

/** \brief This section is dedicated to reversing a specific motor
 *
 * This reverses a specific motor's forward direction only. This function is used to reverse motor direction without any wire hassle.
 * \param motor  - Which motor to reverse.
 * \param value  - True = forward direction is reversed. False = normal operation, no change will be done.
 */
void Ddrive::reverseMotor(const unsigned char motor, const bool value)
{
	switch(motor)
	{
		case 1:
			reverseLeftMotor = value;
			break;
		case 2:
			reverseRightMotor = value;
			break;
		case 3:
			reverseMiddleMotor = value;
			break;
	}
}

/** \brief Constrain function for motor output
 *
 * Constrains floats to a value between 1 & -1, and will adjust other values to ensure the resultant force vector is
 * pointing in the proper direction.
 * \param val1 - Float Input 1
 * \param val2 - Float Input 2
 */
void Ddrive::smartConstrain(float &val1, float &val2)
{

	float temp;
	if(val1 > 1)
	{
		temp = val1-1;
		val1 = 1;
		val2 = val2 - temp;
	} else if(val1 < -1)
	{
		temp = val1+1;
		val1 = -1;
		val2 = val2 - temp;
	}

	if(val2 > 1)
	{
		temp = val2-1;
		val2 = 1;
		val1 = val1 - temp;

	} else if(val2 < -1)
	{
		temp = val2+1;
		val2 = -1;
		val1 = val1 - temp;
	}
}

/** \brief Returns a boolean indiciate if the motor's forward direciton is reversed.
 *
 * Refer to constructor function description for motor numbers.
 */
bool Ddrive::getReverseValue(const unsigned char motor)
{
	switch(motor)
	{
		case 1:
			return reverseLeftMotor;
			break;
		case 2:
			return reverseRightMotor;
			break;
		case 3:
			return reverseLeftMotor;
			break;
	}
}
