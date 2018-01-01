#ifndef HS485_h
#define HS485_h
#include <MiniMaestroService.h>
#include <Arduino.h>
/** \brief Controlls HS485 Servos
 *
 * Used to control HS485 servos with the Maestro.
 */
class HS485{
	public:
		HS485(const MiniMaestroService &miniMaestro, const unsigned char pwm_channel);
		unsigned char getChannel();
		void setPosition(const float position);
	
	protected:

	private:
		unsigned int output;
		float position;
		unsigned char pwm_channel;
		MiniMaestroService* miniMaestro;
};
#endif
