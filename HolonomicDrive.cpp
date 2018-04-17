#include <HolonomicDrive.h>
/** \brief Creates a Holonomic Drive with four generic speed controllers.
 *
 * Motor numbers start from the front right motor, and this one will called "motor1." The number process proceeds counter clockwise.
 * So "motor2" will then be the front left motor. "motor3" will be the back left motor. And finally "motor4" will be the back right motor.
 * \param motor1 - front right motor
 * \param motor2 - front left motor
 * \param motor3 - back left motor
 * \param motor4 - back right motor
 */
HolonomicDrive::HolonomicDrive(const Motor & motor1, const Motor & motor2, const Motor & motor3, const Motor & motor4)
{
	this->motor1 = &motor1;
	this->motor2 = &motor2;
	this->motor3 = &motor3;
	this->motor4 = &motor4;
}

/** \brief This is the holonomic drive, which drives by giving us the direction, thrust, and turn values.
 * \param dir     - The direction we drive towards, this is measured in radians.
 * \param thrust  - This is how much power is to be given to the motors. The input value range is -1 to 1. If the value is zero, the motors
 won't be given any power. The thrust is calculated with vector math.
 * \param turn    - This is the turning factor. Input range is -1 to 1, if zero, no turning will be done.
 */
void HolonomicDrive::drive(const float dir, const float thrust, const float turnfactor)
{
	// Thrust scalers
	float W1thrust = Vec2::dot(Vec2(-.707, .707), Vec2(cos(dir), sin(dir)));
	float W2thrust = Vec2::dot(Vec2(.707, .707), Vec2(cos(dir), sin(dir)));
	// Absolute Value Holders
	float W1abs = fabs(W1thrust);
	float W2abs = fabs(W2thrust);

	// Scale to full thrust
	// TODO: swap Redundant Multipliers with negate logic
	if(W1abs > W2abs)
	{
		W1thrust = W1thrust/W1abs;
		W2thrust = W2thrust/W1abs;
	} else {
		W1thrust = W1thrust/W2abs;
		W2thrust = W2thrust/W2abs;
	}

	// Scale to thrust
	W1thrust = W1thrust * thrust;
	float W3thrust = W1thrust;


	W2thrust = W2thrust * thrust;
	float W4thrust = W2thrust;

	W1thrust = W1thrust + turnfactor;
	W3thrust = W3thrust - turnfactor;

	W2thrust = W2thrust - turnfactor;
	W4thrust = W4thrust + turnfactor;

	smartConstrain(W1thrust, W3thrust);
	smartConstrain(W2thrust, W4thrust);

	// Might be best just to use inverse multipliers instead of logic statements.
	if(reverse1)
	{
		motor1->setPower(-W1thrust);
	} else {
		motor1->setPower(W1thrust);
	}

	if(reverse2)
	{
		motor2->setPower(-W2thrust);
	} else {
		motor2->setPower(W2thrust);
	}

	if(reverse3)
	{
		motor3->setPower(-W3thrust);
	} else {
		motor3->setPower(W3thrust);
	}

    //Constrains floats between 1 & -1, will adjust other values to ensure resultant force vector is pointing in proper direction.
    if(reverse4)
	{
		motor4->setPower(-W4thrust);
	} else {
		motor4->setPower(W4thrust);
	}

}


/** \brief Constrain function for motor output
 *
 * Constrains floats to a value between 1 & -1, and will adjust other values to ensure the resultant force vector is
 * pointing in the proper direction.
 * \param val1 - Float Input 1
 * \param val2 - Float Input 2
 */
void HolonomicDrive::smartConstrain(float &val1, float &val2)
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
	}//Constrains floats between 1 & -1, will adjust other values to ensure resultant force vector is pointing in proper direction.

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

/** \brief This section is dedicated to reversing a specific motor
 *
 * This reverses a specific motor's forward direction only. This function is used to reverse motor direction without any wire hassle.
 * \param motor  - Which motor to reverse.
 * \param value  - True = forward direction is reversed. False = normal operation, no change will be done.
 */
void HolonomicDrive::reverseMotor(const unsigned char motor, const bool value)
{
	switch(motor)
	{
		case 1:
			reverse1 = value;
		case 2:
			reverse2 = value;
		case 3:
			reverse3 = value;
		case 4:
			reverse4 = value;
	}
}

/** \brief This reverses the left motor's forward direction.
 *
 * Reverses the motor's forward direction. This function is used if we need to reverse the forward direction and if we want no wiring hassle.
 * \param value  - True = reverse. False = normal operation.
 */
void HolonomicDrive::reverseLeftMotors(const bool value)
{//They're 2 and 3 because the left motors are motors 2 and 3.
	reverse2 = value;
	reverse3 = value;
}

/** \brief This reverses the right motor's forward direction.
 *
 * Reverses the motor's forward direction. This function is used if we need to reverse the forward direction and if we want no wiring hassle.
 * \param value  - True = reverse. False = normal operation.
 */
void HolonomicDrive::reverseRightMotors(const bool value)
{//They're 1 and 4 because the right motors are motors 1 and 4
	reverse1 = value;
	reverse4 = value;
}
