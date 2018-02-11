#include <Ddrive.h>

Ddrive::Ddrive(Motor &leftmotor, Motor &rightmotor, Motor &middlemotor)
{
	this->leftmotor = &leftmotor;
	this->rightmotor = &rightmotor;
	this->middlemotor = &middlemotor;
}
 
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
	Mthrust = Mthrust * thrust + turnfactor;

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
