#ifndef BATTERY_METER_H
#define BATTERY_METER_H

/**
 * Initialize battery measurement system.
 */
void batteryMeterInit(void);

/**
 * Get battery's voltage.
 *
 * \return Battery voltage.
 */
float getBatteryVoltage(void);

#endif
