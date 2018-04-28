#include <Vector_temp.h>
/** \brief Constructs 2d vector
 * \param x x-component
 * \param y y-component
 */
Vec2::Vec2(float x, float y)
{
	components[0] = x;
	components[1] = y;
}

/** \brief Dot Product Operation
 * Calculates the dot product of two vectors.
 * \param value1 First Vector
 * \param value2 Second Vector
 * \return Dot Product Value
 */
static float Vec2::dot(Vec2 value1, Vec2 value2)
{
	return value1.components[0]*value2.components[0] + value1.components[1]*value2.components[1];
}

/** \brief Magnitude of a Vector
 * Calculates the magnitude(length) of a vector.
 * \param value Input Vector
 * \return Length of Input Vector
 */
static float Vec2::magnitude(Vec2 value)
{
	return sqrt(sq(value.components[0]) + sq(value.components[1]));
}

/** \brief Calculate angle of a vector.
 * Measured from positive x-axis using law of cosines. CCW is positive direction.
 * \param value Input Vector
 * \return Angle in radians, measured from positive x-axis.
 */
static float Vec2::angle(Vec2 value)
{
	return atan2(value.components[1], value.components[0]);
}

/** \brief Calculate angle of a vector, in degrees.
 * Measured from positive x-axis using law of cosines. CCW is positive direction.
 * \param value Input Vector
 * \return Angle in degrees, measured from positive x-axis.
 */
static float Vec2::angled(Vec2 value)
{
	return atan2(value.components[0], value.components[1])*180/3.14;
}
