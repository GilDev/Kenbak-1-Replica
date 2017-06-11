#ifndef CONFIG_H
#define CONFIG_H

/**
 * Activate serial debugging.
 */
#define DEBUG

/**
 * Serial baud.
 */
#define SERIAL_BAUD 115200

/**
 * Buttons pins connected to the MCP23017.
 */
enum InputPins {
	I_B0              = 8,
	I_B1              = 9,
	I_B2              = 10,
	I_B3              = 11,
	I_B4              = 12,
	I_B5              = 13,
	I_B6              = 14,
	I_B7              = 15,
	I_CLEAR           = 1,
	I_DISPLAY_ADDRESS = 2,
	I_SET_ADDRESS     = 3,
	I_READ_MEMORY     = 7,
	I_STORE_MEMORY    = 6,
	I_START           = 5,
	I_STOP            = 4,
};
/**
 * Memory lock toggle switch pin connected to the Arduino.
 */
#define I_MEMORY_LOCK 6

/**
 * LEDs pins connected to the TLC5940NT.
 */
enum LedsPins {
	L_7       = 9,
	L_6       = 8,
	L_5       = 7,
	L_4       = 6,
	L_3       = 5,
	L_2       = 4,
	L_1       = 3,
	L_0       = 1,
	L_INPUT   = 2,
	L_ADDRESS = 14,
	L_MEMORY  = 13,
	L_RUN     = 12
};

/**
 * Analog input where the battery's voltage divider is connected.
 * 
 * 10k/3.3k so 4.2 V -> 1.04 V -> 968 ADC value, 3.3 V -> 0,82 V -> 763 ADC value.
 * Better resistor values could probably be chosen to have a more precise range around 3-4.5 V.
 */
#define BATTERY_METER_PIN A0

/**
 * Kenbak's CPU frequency.
 * Just uses `delay()` in the background. Not very precise.
 */
#define CPU_FREQ 300 // In Hz

#endif
