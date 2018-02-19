// TODO 2-15
// CHECK PROG USAGE
// Currently Working on PUSHING 3vairables onto stacks

#ifndef OSUBSYSTEMS_H
#define OSUBSYSTEMS_H
#include <TalonSR.h>
#include <PololuG2.h>
#include <HolonomicDrive.h>
#include <HS485.h>
#include <Motor.h>
#define MAX_SEQUENCE 20
#define INTAKE_MOTOR_POWER 1
#define INTAKE_SERVO_ANGLE 2
#define INTAKE_INTAKE 3
#define INTAKE_OUTTAKE 4
#define INTAKE_IDLE 5 
#define SHOOTER_ANGLE 6
#define SHOOTER_POWER 7 
#define CHAMBER_INTAKE 8 
#define CHAMBER_SHOOT 9 
#define CHAMBER_IDLE 10
#define RESET_SUBSYSTEMS 11

class OSubsystems{
	public:
		OSubsystems(const HolonomicDrive & drive, const Motor & shooter, const HS485 & shooter_servo, const HS485 & chamber, const Motor & intake_motor, const HS485 & intake_servo);
		
		// Sequence Commands
		void pushSequence(byte type, unsigned long delay, bool executeNow); 
		void pushSequence(byte type, unsigned long delay, float value, bool executeNow);
		void popSequence();
		void executeSequence();
		void resetSubsystems();
		void iterate();
		bool isLocked();
		void resetLocks();

		// Subsystems Command
		void setShooter(float power, bool bypass = false);
		void setShooterAngle(const float angle, bool bypass = false);
		void setChamber(float angle, bool bypass = false);
		void setIntakeAngle(float angle, bool bypass = false);
		void setIntakeRoller(float power, bool bypass = false);
		void setSystemsIntake(bool bypass = false);
		void setSystemsOuttake(bool bypass = false);
		void setSystemsIdle(bool bypass = false);

		// Default Values
		float chamber_intake_pos;
		float chamber_shoot_pos;
		float chamber_idle_pos;
		float intake_idle_pos;
		float intake_intake_pos;
		float intake_roller_in;
		float intake_roller_out;
	protected:
	private:
		// Object Pointers
		HolonomicDrive* chassis;
		Motor* shooter;
		Motor* intake_motor;
		HS485* intake_servo;
		HS485* chamber;
		HS485* shooter_servo;

		// Sequence Locks
		bool sequenceLock;
		bool autolock = true;
		bool chamberLock;
		bool intakeMotorLock;
		bool intakeServoLock;
		bool shooterLock;

		// Sequence Data
		byte sequence_types[MAX_SEQUENCE];
		unsigned long sequence_delays[MAX_SEQUENCE];
		float sequence_values[MAX_SEQUENCE];
		byte sequence_index;
		unsigned long last_time;
};
#endif
