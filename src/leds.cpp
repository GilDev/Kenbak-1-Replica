#include "config.h"
#include "cpu.h"
#include "inputs.h"
#include "leds.h"
#include "modes.h"

Tlc5940 tlc;

const uint8_t leds[] = {L_7, L_6, L_5, L_4, L_3, L_2, L_1, L_0, L_INPUT, L_ADDRESS, L_MEMORY, L_RUN};
uint16_t ledBrightness, ledLowBrightness;

static void updateDataLeds(uint8_t number, uint16_t brightness)
{
	uint8_t bit;
	for (bit = 0; bit < 8; bit++) {
		if (number & 1 << bit)
			tlc.set(leds[7 - bit], brightness);
	}
}

void ledsInit(void)
{
	tlc.init();
	ledBrightness    = 0xFFF;
	ledLowBrightness = 0x05F;
}

void readyAnimation(void)
{
	uint8_t i;

	for (i = 0; i < sizeof leds; i++) {
		tlc.set(leds[i], ledBrightness);
		tlc.update();
		delay(50);
	}

	for (i = 0; i < sizeof leds; i++) {
		tlc.set(leds[i], 0);
		tlc.update();
		delay(50);
	}
}

void blinkLed(uint8_t ledPin, uint8_t number)
{
	for (; number > 0; number--) {
		tlc.set(ledPin, ledBrightness);
		tlc.update();
		delay(100);
		tlc.set(ledPin, 0);
		tlc.update();
		delay(100);
	}
}

void updateLeds(void)
{
	uint8_t dataLeds = 0;

	tlc.clear();

	switch (mode) {
		case DATA_INPUT:
			tlc.set(L_INPUT, ledBrightness);
			dataLeds = *INPUT_REG;
		break;

		case ADDRESS:
			tlc.set(L_ADDRESS, ledBrightness);
			dataLeds = addressReg;
		break;

		case MEMORY:
			tlc.set(L_MEMORY, ledBrightness);
			dataLeds = *(ram + (addressReg - 1)); // "addressReg - 1" because addressReg already point to the next address
		break;

		case RUN:
			tlc.set(L_RUN, ledBrightness);
		case HALT:
			dataLeds = *OUTPUT_REG;
		break;
	}

	if (buttonPressed[I_STORE_MEMORY] && digitalRead(I_MEMORY_LOCK)) // If you store in memory, light up the "run" LED
		tlc.set(L_RUN, ledBrightness);

	if (mode == RUN && dataButtonPressed) {
		updateDataLeds(dataLeds, ledLowBrightness);
		updateDataLeds(*INPUT_REG, ledBrightness);
	} else {
		updateDataLeds(dataLeds, ledBrightness);
	}

	tlc.update();
}
