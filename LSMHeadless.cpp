#include <LSMHeadless.h>
/** \brief Sets the relative yaw to zero.
 *
 * Sets the relative yaw to zero. You can use this to reoriente the forward direction of headless mode.
 */
void LSMHeadless::zero()
{
	relative_yaw = 0;
}

/** \brief Calibrates the gryo.
 *
 * Takes a few samples to offset gyro drift.
 *
 * \warning This function will not return untill the gryo is done calibrating. Beware of runaway robots.
 */
void LSMHeadless::calibrate()
{
	if(!isInitialized)
		return;
	float total;
	float sample;
	for(unsigned char i = 0; i < 150; i++)
	{
		lsm6.read();
		total = total + lsm6.g.z;
		sample++;
		delay(5);
	}
	bias = total/sample;
	prev_time = millis(); // Updates time differential.
}

/** \brief Initializes the gryo.
 *
 * Default settings: 104Hz at 2000dps.
 */
void LSMHeadless::init()
{
	Wire.begin();
	delay(10);
	tries = 0;
	while(!(isInitialized = lsm6.init()) && tries < 150)
	{
		tries++;
		delay(5);
	}

	lsm6.enableDefault();
  	lsm6.writeReg(LSM6::CTRL1_XL, 0x3C); // 52 Hz, 8 g full scale
	lsm6.writeReg(LSM6::CTRL2_G, 0x4C); // 104 Hz, 2000 dps full scale
}	

/** \brief Integrates gryo data. Should be call periodically.
 *
 * Integrate gyro using first order forward integration. Should be called periodically (minimum 20Hz).
 */
void LSMHeadless::iterate()
{
	if(!isInitialized)
		return;
	float timer = millis();
	float dt = float(timer - prev_time) / 1000;
	lsm6.read();
	float delta_yaw = float(lsm6.g.z - bias)*dt*70*0.001;// Formula derived from LSM6 conversion factors. Checkout with Allen for more info.
	abs_yaw = abs_yaw + delta_yaw;
	relative_yaw = relative_yaw + delta_yaw;
	prev_time = timer;
}

/** \brief Returns absolute yaw.
 *
 * Returns absolute yaw. In this context, absolute yaw is the total angular displacment of the gryo. Absolute yaw can not be reset.
 *
 * \return Total Angular Displacment - The amount of angular displacment in degrees.	
 */
float LSMHeadless::getAbsYaw()
{
	return abs_yaw;
}

/** \brief Returns relative yaw.
 *
 * Returns relative yaw. In this context, relative yaw is the total angular displacment of the gryo in respect to the zero() reference. Calling zero() will set relative yaw to zero.
 *
 * \return Angular Displacment - The angular displacment with respect to the zero reference, in degrees.
 */
float LSMHeadless::getRelativeYaw()
{
	return relative_yaw;
}

/** \brief Returns gyro bias.
 *
 * Gyro bias is associated with gyro drift. The larger the bias, the faster the gryo will drift. The bias value is calculated upon calling calling calibrate(), and is then compensated for when calling iterate().
 *
 * \return Bias - Raw gryo value that represents bias.
 */
int16_t LSMHeadless::getBias()
{
	return bias;
}

/** \brief Returns LSM6 reference.
 *
 * Returns the LSM6 (gryo/accl) hanndler.
 *
 * \return LSM6 Handler
 */
LSM6* LSMHeadless::getLSM6()
{
	return &lsm6;
}

/** \brief Trims the relative yaw.
 *
 * \param value	- Amount to adjust relative yaw by, in degrees.
 */
void LSMHeadless::trim(float value)
{
	relative_yaw = relative_yaw + value;
}

/** \brief Returns the initailizaiton LSM6 initialization state.
 *
 * After init() is succesfully called, this function will return true.
 *
 * \return IsInitialized	- True: LSM6 is initialized, False: LSM6 is not initialized.
 */
bool LSMHeadless::getInitialized()
{
	return isInitialized;
}
