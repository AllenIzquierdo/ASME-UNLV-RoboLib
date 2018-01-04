#include <HolonomicDrive.h>
/** \brief Creates Holonomic Drive with four generic speed controllers.
 *
 * Motor number starts from front right motor being "motor1." Then proceeds counter clockwise. "motor2" will then be front left motor. "motor3" will be backleft motor. "motor4" will be back right motor.
 * \param motor1 front right motor
 * \param motor2 front left motor
 * \param motor3 back left motor
 * \param motor4 back right motor 
 */
HolonomicDrive::HolonomicDrive(const Motor & motor1, const Motor & motor2, const Motor & motor3, const Motor & motor4)
{
	this->motor1 = &motor1;
	this->motor2 = &motor2;
	this->motor3 = &motor3;
	this->motor4 = &motor4;
}

/** \brief Drives holonomic drive with direction, thrust, and turn.
 * \param dir Direction to drive towards, measured in radians.
 * \param thrust How much power should be given to the motors. Input value range is -1 to 1.
 * \param turn Turning factor. Input range is -1 to 1.
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
	if(W1abs > W2abs)
	{
		W1thrust = 1;
		W2thrust = W2thrust/W1abs;
	} else {
		W1thrust = W1thrust/W2abs;
		W2thrust = 1;
	}

	// Scale to thrust
	W1thrust = W1thrust * thrust;
	float W3thrust = W1thrust;


	W2thrust = W2thrust * thrust;
	float W4thrust = W2thrust;

	W1thrust = W1thrust + turnfactor;
	W3thrust = W3thrust - turnfactor;

	W2thrust = W2thrust - turnfactor;
	W4thrust = W2thrust + turnfactor;
	smartConstrain(W1thrust, W3thrust);
	smartConstrain(W2thrust, W4thrust);
	motor1->setPower(W1thrust);
	motor2->setPower(W2thrust);
	motor3->setPower(W3thrust);
	motor4->setPower(W4thrust);
}

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
