#ifndef POLOLUG2_H
#define POLOLUG2_H
#define POLOLUG2_MAX_LINEAR_RAMP 10
#include <Arduino.h>
#include <Motor.h>
#include <MiniMaestroService.h>
#include <math.h>

/** \brief Controlls PololuG2 motor drivers.
 *
 * Got two options in controlling PololuG2 motors.
 * 1. PWM analogWrite with arduino.
 * 2. Maestro with 250Hz update rate.
 *
 * When using option 2, use Maestro Control Center to set enable and direction channels to output, and PWM pin as a servo.
 */
class PololuG2: public Motor {
	public:
		PololuG2(const byte enable_pin, const byte pwm_pin, const byte dir_pin, const bool linearRamping = false);
		PololuG2(const MiniMaestroService &miniMaestroService, const byte enable_channel, const byte pwm_channel, const byte dir_channel, const bool linearRamping = false);
		
		// Setters and Getters
		void setPower(const float power);
		void setOutputPower(const float power);
		void setVelocity(const float velocity);
		float getVelocity();
		float getTarget();
		static void iterate();
	protected:
	private:
		// Pins/Channels
		byte enable_pin;
		byte pwm_pin;
		byte dir_pin;

		// MiniMaestro Protocal
		byte output;
		unsigned int maestroOutput;
		bool maestroService;
		MiniMaestroService* miniMaestroService;

		// Linear Ramping
		static void addLinearRampingMotor(PololuG2* motor);
		bool linearRamping;
		float target;
		float velocity = 4;
		static float last_update;
		static unsigned char motor_count;
		static PololuG2* motors[10];
};

#endif
