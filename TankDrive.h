
#ifndef TANKDRIVE_H
#define TANKDRIVE_H
#include <Motor.h>
#include <PS2X_lib.h>


/** \brief Convenient TankDrive class.
 *
 * Encapsulation for TankDrive code.
 */
class TankDrive {
	public:
		TankDrive(const Motor &leftMotor1, const Motor &leftMotor2, const Motor &rightMotor1, const Motor &rightMotor2);
		TankDrive(const Motor &leftMotor1, const Motor &leftMotor2);
		void drive(PS2X ps2x);
		void drive(float left, float right);
		void reverseLeftMotors(bool enable);
		void reverseRightMotors(bool enable);
	protected:
	private:
		bool reverseLeft;
		bool reverseRight;
		bool doubleDriveEnable = true; // Enable By Default, disabled by constructor.
		Motor* leftMotor1;
		Motor* leftMotor2;
		Motor* rightMotor1;
		Motor* rightMotor2;
};

#endif
