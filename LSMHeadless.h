#ifndef LSMHEADLESS.H
#define LSMHEADLESS.H
#include <Wire.h>
#include <LSM6.h>
#include <Arduino.h>

/** \brief This object handles communication and integration of gryodata.
 *
 * This object is meant to interface with Pololu's MinIMU-9 v5 (Pololu #2738). It initializes communication and integrates gyro data to track relative angles about the z-axis.
 */
class LSMHeadless{
	public:
		void init();
		void zero();
		void iterate();
		void calibrate();
		void trim(float);
		float getAbsYaw();
		float getRelativeYaw();
		int16_t getBias();
		LSM6* getLSM6();
		bool getInitialized();

		unsigned char tries;

	protected:
	private:
		bool isInitialized = false;
		unsigned long calibration_time;
		unsigned long last_calibration;
		float abs_yaw;
		float relative_yaw;
		unsigned long prev_time;
		int16_t bias;
		LSM6 lsm6;
};

#endif
