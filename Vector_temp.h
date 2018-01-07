
#ifndef VECTOR_TEMP.H
#define VECTOR_TEMP.H
#include <Arduino.h>
#include <math.h>
/** \brief Generic Costly 2D Vector Class
 *
 * At some point, we'll have to implement EigenArduino library. Untill then, these makeshift classes will have to carry us to victory.
 * \warning These are costly functions. Use with caution.
 */
class Vec2{
	public:
		Vec2(float x, float y);
		static float dot(Vec2 value1, Vec2 value2);
		static float magnitude(Vec2 value);
		static float angle(Vec2 value);
		static float angled(Vec2 value);
		float components[2];
	protected:
	private:
};

#endif
