#ifndef OSUBSYSTEMS_H
#define OSUBSYSTEMS_H
#include <TalonSR.h>
#include <PololuG2.h>
#include <HolonomicDrive.h>
#include <HS485.h>
class OSubsystems{
	public:
		OSubSystems(HolonomicDrive &drive, Motor &shooter, HS485 &chamber, Motor &intakeRoller, HS485 intakeHS485);
		void pushSequence(byte type, unsigned long delay, bool executeNow);
		void pushSequence(byte type, unsigned long delay, float value, bool executeNow);
		void popSequence();
		void executeSequence();
		void resetSubsystems();
		void iterate();
		void chamberIntake();
		void setShooter(float power);
		void setChamber(float angle);
		void setIntakeAngle(float angle);
		void setIntakeRoller(float power);
	protected:
	private:
		HolonomicDrive* chassis;
		TalonSR* shooter;
		HS485* chamber;
		bool sequenceLock;
		float chamber_intake_pos;
		float chamber_shoot_pos;
		float chamber_idle_pos;
		float intake_idle_pos;
		float intake_intake_pos;
		byte sequence_index_type;
		byte sequence_index_value;
		byte sequence_index

}
#endif
