#include <LSMHeadless.h>
/** \brief Set's new heading 
 */
void LSMHeadless::zero()
{
	relative_yaw = 0;
}

void LSMHeadless::calibrate()
{
	if(!isInitilized)
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

void LSMHeadless::init()
{
	Wire.begin();
	unsigned byte tries;
	while(!(isInitialized == lsm6.init()) && tries < 100)
	{
		tries++;
		delay(5);
	}
	lsm6.enableDefault();
  	lsm6.writeReg(LSM6::CTRL1_XL, 0x3C); // 52 Hz, 8 g full scale
	lsm6.writeReg(LSM6::CTRL2_G, 0x4C); // 104 Hz, 2000 dps full scale
}	

void LSMHeadless::iterate()
{
	if(!isInitilized)
		return;
	float timer = millis();
	float dt = float(timer - prev_time) / 1000;
	lsm6.read();
	float delta_yaw = float(lsm6.g.z - bias)*dt*70*0.001;// Formula derived from LSM6 conversion factors. Checkout with Allen for more info.
	abs_yaw = abs_yaw + delta_yaw;
	relative_yaw = relative_yaw + delta_yaw;
	prev_time = timer;
}

float LSMHeadless::getAbsYaw()
{
	return abs_yaw;
}

float LSMHeadless::getRelativeYaw()
{
	return relative_yaw;
}

int16_t LSMHeadless::getBias()
{
	return bias;
}

LSM6* LSMHeadless::getLSM6()
{
	return &lsm6;
}

void LSMHeadless::trim(float value)
{
	relative_yaw = relative_yaw + value;
}
