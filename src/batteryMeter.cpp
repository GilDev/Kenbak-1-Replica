#include <Arduino.h>
#include "config.h"

void batteryMeterInit(void)
{
	pinMode(A0, INPUT);
	analogReference(INTERNAL);
}

float getBatteryVoltage(void)
{
	return (float) (analogRead(BATTERY_METER_PIN) * 55) / 12800.0;
}
