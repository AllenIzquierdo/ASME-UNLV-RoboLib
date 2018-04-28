#include <OSubsystems.h>

/** \brief Offensive Subsystem Constructor
 *
 * Requires all actuators to be constructed prior to Subsystem Construciton.
 * \param &drive Drive Chassis
 * \param &shooter Flywheel Shooter Motor
 * \param &shooter_servo Servo that controls projectile angle.
 * \param &chamber Servo that actuates the ball chamber. Or as Patrick calls it, the "reservoir."
 * \param &intake_motor Motor that actuates intake belt.
 * \param &intake_servo Servo that controls the rolling intake angle.
 */
OSubsystems::OSubsystems(const HolonomicDrive &drive, const Motor &shooter, const HS485 &shooter_servo, const HS485 &chamber, const Motor &intake_motor, const HS485 &intake_servo)
{
	this->chassis = &drive;
	this->shooter = &shooter;
	this->shooter_servo = &shooter_servo;
	this->chamber = &chamber;
	this->intake_motor = &intake_motor;
	this->intake_servo = &intake_servo;
}

/** \brief Pushes a command onto the sequence stack.
 *
 * The fundamental function for creating a sequence stack. If the command includes an additional argument, e.g. SHOOTER_POWER, it will execute with an argument of zero.
 * See competition deployments for example usage.
 *
 * \param type The command that will be added to the sequence stack. See macro declerations.
 * \param delay The amount of time in milliseconds that should elaspe since the last executed sequence stack command. A delay of 0 microseconds will execute the command immediately.
 * \param executeNow Starts the command sequence. 
 */
void OSubsystems::pushSequence(byte type, unsigned long delay, bool executeNow = false)
{
	pushSequence(type, delay, 0, executeNow);
}

/** \brief Pushes a command onto the sequence stack.
 *
 * The fundamental function for creating a sequence stack.
 * See competition deployments for example usage.
 *
 * \param type The command that will be added to the sequence stack. See macro declerations.
 * \param delay The amount of time in milliseconds that should elaspe since the last executed sequence stack command. A delay of 0 microseconds will execute the command immediately.
 * \param value The value that will be passed to the command.
 * \param executeNow Starts the command sequence. 
 */
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

/** \brief Immediately pops a command from the sequence stack and executes it.
 */
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

/** \brief Begin popping commands from the sequence stack automatically. 
 *
 * After this function is called, the user can not push any more commands onto the sequence stack untill it emptys.
 */
void OSubsystems::executeSequence()
{
	sequenceLock = true;
	last_time = millis();
}

/** \brief place holder
 */
void OSubsystems::resetSubsystems()
{
}

/** \brief Handles sequence housekeeping.
 *
 * Checks if a sequence command should be executed. If it should, it would execute said command.
 * \warning This function must be called periodically for sequence stack to work. I reccomend calling this function at 20Hz.
 */
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

/** \brief Sets the shooter motor power.
 * 
 * Sets the shooter motor power
 *
 * \param power Motor power value. See Motor class.
 * \param bypass If true, the command will be executed even if the subsystem is locked.
 */
void OSubsystems::setShooter(float power, bool bypass = false)
{
	// If sequence occuring and user is not bypassing, cancel the command.
	if(shooterLock && !bypass)
		return;
	shooter->setPower(power);
}

/** \brief Sets the shooter servo angle
 *
 * \param angle Servo angle value. See HS485 class.
 * \param bypass If true, the command will be executed even if the subsystem is locked.
 */
void OSubsystems::setShooterAngle(float angle, bool bypass = false)
{
	if(shooterLock && !bypass)
		return;
	shooter_servo->setPosition(angle);
}

/** \brief Sets the chamber servo angle
 *
 * \param angle Servo angle value. See HS485 class.
 * \param bypass If true, the command will be executed even if the subsystem is locked.
 */
void OSubsystems::setChamber(float angle, bool bypass = false)
{
	if(chamberLock && !bypass)
		return;
	chamber->setPosition(angle);
}

/** \brief Sets the intake servo angle
 *
 * \param bypass If true, the command will be executed even if the subsystem is locked.
 */
void OSubsystems::setIntakeAngle(float angle, bool bypass = false)
{
	if(intakeServoLock && !bypass)
		return;
	intake_servo->setPosition(angle);
}

/** \brief Sets the intake servo angle
 *
 * \param power Motor power value. See Motor class.
 * \param bypass If true, the command will be executed even if the subsystem is locked.
 */
void OSubsystems::setIntakeRoller(float power, bool bypass = false)
{
	if(intakeMotorLock && !bypass)
		return;
	intake_motor->setPower(power);
}

/** \brief Sets the Intake Roller, Intake Servo, and Chamber Servo to intake a ball.
 *
 * \param bypass If true, the command will be executed even if the subsystem is locked.
 */
void OSubsystems::setSystemsIntake(bool bypass = false)
{
	setIntakeAngle(intake_intake_pos, bypass);
	setIntakeRoller(intake_roller_in, bypass);
	setChamber(chamber_intake_pos, bypass);
}

/** \brief Sets the Intake Roller, Intake Servo, and Chamber Servo to outtake ball.
 *
 * \param bypass If true, the command will be executed even if the subsystem is locked.
 */
void OSubsystems::setSystemsOuttake(bool bypass = false)
{
	setIntakeAngle(intake_intake_pos, bypass);
	setIntakeRoller(intake_roller_out, bypass);
	setChamber(chamber_intake_pos, bypass);
}

/** \brief Sets the Intake Roller, Intake Servo, and Chamber Servo to idle position.
 *
 * \param bypass If true, the command will be executed even if the subsystem is locked.
 */
void OSubsystems::setSystemsIdle(bool bypass = false)
{
	setIntakeAngle(intake_idle_pos, bypass);
	setIntakeRoller(0, bypass);
	setChamber(chamber_idle_pos, bypass);
}

/** \brief Resets Subsystem Locks
 */
void OSubsystems::resetLocks()
{
	chamberLock = false;
	intakeMotorLock = false;
	intakeServoLock = false;
	shooterLock = false;
}

/** \brief Check if sequence lock.
 *
 * The sequence is locked whenever pushSequence() is called with executeNow = true or executeSequence() is called. When the sequence stack empties, the sequenceLock = true.
 */
bool OSubsystems::isLocked()
{
	return sequenceLock;
}
