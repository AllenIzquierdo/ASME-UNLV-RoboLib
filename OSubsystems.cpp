#include <OSubsystems.h>

OSubsystems::OSubsystems(const HolonomicDrive &drive, const Motor &shooter, const HS485 &shooter_servo, const HS485 &chamber, const Motor &intake_motor, const HS485 &intake_servo)
{
	this->chassis = &drive;
	this->shooter = &shooter;
	this->shooter_servo = &shooter_servo;
	this->chamber = &chamber;
	this->intake_motor = &intake_motor;
	this->intake_servo = &intake_servo;
}

void OSubsystems::pushSequence(byte type, unsigned long delay, bool executeNow = false)
{
	pushSequence(type, delay, 0, executeNow);
}

void OSubsystems::pushSequence(byte type, unsigned long delay, float value, bool executeNow)
{
	if(sequenceLock)
		return;
	// Switch Statments is a bad programming practice. If you find this, I give you permission to crucify me for this sin.
	if(autolock)
	{
		switch(type)
		{
			case CHAMBER_INTAKE:
			case CHAMBER_SHOOT:
			case CHAMBER_IDLE:
				chamberLock = true;
				break;
			case INTAKE_MOTOR_POWER:
				intakeMotorLock = true;
				break;
			case INTAKE_SERVO_ANGLE:
				intakeServoLock = true;
				break;
			case INTAKE_INTAKE:
			case INTAKE_OUTTAKE:
				intakeServoLock = true;
				intakeServoLock = true;
				break;
			case SHOOTER_POWER:
			case SHOOTER_ANGLE:
				shooterLock = true;
				break;
		}
	}
	
	// Pushes a sequence on sequence stack.
	if(sequence_index < MAX_SEQUENCE)
	{
		sequence_types[sequence_index] = type;
		sequence_delays[sequence_index] = delay;
		sequence_values[sequence_index] = value;
		sequence_index++;
	}

	// begins sequence.
	if(executeNow)
	{
		executeSequence();
	}
	
}

// TODO FILL Sequences
void OSubsystems::popSequence()
{
	byte type = sequence_types[sequence_index];
	float value = sequence_values[sequence_index];
	switch(type)
	{
		case SHOOTER_ANGLE:
			setShooterAngle(value, true);
			break;
		case SHOOTER_POWER:
			setShooter(value, true);
			break;
		case CHAMBER_SHOOT:
			setChamber(chamber_shoot_pos, true);
			break;
		case CHAMBER_INTAKE:
			setChamber(chamber_intake_pos, true);
			break;
		case CHAMBER_IDLE:
			setChamber(chamber_idle_pos, true);
			break;
		case RESET_SUBSYSTEMS:
			break;
		case INTAKE_MOTOR_POWER:
			setIntakeRoller(value, true);
			break;
		case INTAKE_SERVO_ANGLE:
			setIntakeAngle(value, true);
			break;
		case INTAKE_INTAKE:
			setSystemsIntake(true);
			break;
		case INTAKE_OUTTAKE:
			setSystemsOuttake(true);
			break;
		case INTAKE_IDLE:
			setSystemsIdle(true);
			break;
	}
	sequence_index--;
}

void OSubsystems::executeSequence()
{
	sequenceLock = true;
	last_time = millis();
}

void OSubsystems::resetSubsystems()
{
}

void OSubsystems::iterate()
{
	// No Sequence is running? Then don't iterate.
	if(!sequenceLock)
		return;

	// Pops Sequence When delay time has passed or delay == 0.
	if((millis() - last_time) > sequence_delays[sequence_index])
	{
		popSequence();
		while(sequence_delays[sequence_index] == 0 && sequence_index >= 0)
		{
			popSequence();
		}
		last_time = millis();
	}

	// Turns off Sequence & release subsystem controlls.
	if(sequence_index < 0)
	{
		sequenceLock = false;
		resetLocks(); 
		sequence_index = 0;
	}
}

void OSubsystems::setShooter(float power, bool bypass = false)
{
	// If sequence occuring and user is not bypassing, cancel the command.
	if(shooterLock && !bypass)
		return;
	shooter->setPower(power);
}

void OSubsystems::setShooterAngle(float angle, bool bypass = false)
{
	if(shooterLock && !bypass)
		return;
	shooter_servo->setPosition(angle);
}

void OSubsystems::setChamber(float angle, bool bypass = false)
{
	if(chamberLock && !bypass)
		return;
	chamber->setPosition(angle);
}

void OSubsystems::setIntakeAngle(float angle, bool bypass = false)
{
	if(intakeServoLock && !bypass)
		return;
	intake_servo->setPosition(angle);
}

void OSubsystems::setIntakeRoller(float power, bool bypass = false)
{
	if(intakeMotorLock && !bypass)
		return;
	intake_motor->setPower(power);
}

void OSubsystems::setSystemsIntake(bool bypass = false)
{
	setIntakeAngle(intake_intake_pos, bypass);
	setIntakeRoller(intake_roller_in, bypass);
	setChamber(chamber_intake_pos, bypass);
}

void OSubsystems::setSystemsOuttake(bool bypass = false)
{
	setIntakeAngle(intake_intake_pos, bypass);
	setIntakeRoller(intake_roller_out, bypass);
	setChamber(chamber_intake_pos, bypass);
}

void OSubsystems::setSystemsIdle(bool bypass = false)
{
	setIntakeAngle(intake_idle_pos, bypass);
	setIntakeRoller(0, bypass);
	setChamber(chamber_idle_pos, bypass);
}

// Releases Sequence Controll of Subsystems
void OSubsystems::resetLocks()
{
	chamberLock = false;
	intakeMotorLock = false;
	intakeServoLock = false;
	shooterLock = false;
}

bool OSubsystems::isLocked()
{
	return sequenceLock;
}
