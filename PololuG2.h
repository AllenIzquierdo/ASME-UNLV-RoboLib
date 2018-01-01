#ifndef POLOLUG2_H
#define POLOLUG2_H
#include <Arduino.h>
#include <Motor.h>
#include <MiniMaestroService.h>

/** \brief Controlls PololuG2 motor drivers.
 *
 * Got two options in controlling PololuG2 motors.
 * 1. PWM analogWrite with arduino.
 * 2. Maestro with 333Hz update rate.
 *
 * When using option 2, use Maestro Control Center to set enable and direction channels to output, and PWM pin as a servo.
 */
class PololuG2: public Motor {
	public:
		PololuG2(const byte enable_pin, const byte pwm_pin, const byte dir_pin);
		PololuG2(const MiniMaestroService &miniMaestroService, const byte enable_channel, const byte pwm_channel, const byte dir_channel);
		
		// Setters and Getters
		void setPower(const float power);
	protected:
	private:
		byte output;
		unsigned int maestroOutput;
		bool maestroService;
		MiniMaestroService* miniMaestroService; 
		byte enable_pin;
		byte pwm_pin;
		byte dir_pin;
};

#endif
