#ifndef HS485_h
#define HS485_h
#include <MiniMaestroService.h>
#include <Arduino.h>
/** \brief This controls the HS485 Servos
 *
 * This is used to control the HS485 servos with the Maestro.
 * \warning Keep in mind that the maestro is pulsing the servo at 50Hz. Pulsing the servo at values above 50Hz can damage the servo!
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
