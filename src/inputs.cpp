#include <EEPROM.h>
#include "config.h"
#include "cpu.h"
#include "inputs.h"
#include "leds.h"
#include "modes.h"
#include "samplePrograms.h"

Adafruit_MCP23017 inputs;

bool buttonPressed[16] = {false};
bool dataButtonPressed = false;

void inputsInit(void)
{
	inputs.begin();

	uint8_t input;
	for (input = 0; input < 16; input++) {
		inputs.pinMode(input, INPUT);
		inputs.pullUp(input, HIGH);
	}

	pinMode(I_MEMORY_LOCK, INPUT_PULLUP);
}

void handleInputs(void)
{
	uint8_t button;
	uint8_t bitPressed = 0xFF;

	for (button = 0; button < 16; button++) {
		if (!inputs.digitalRead(button)) {
			if (!buttonPressed[button]) {
				buttonPressed[button] = true;

				switch (button) {
					case I_B0:
					case I_B1:
					case I_B2:
					case I_B3:
					case I_B4:
					case I_B5:
					case I_B6:
					case I_B7:

						// This is ugly! Need to find something else, like the `leds` array but for bit buttons
						switch (button) {
							case I_B0: bitPressed = 0; break;
							case I_B1: bitPressed = 1; break;
							case I_B2: bitPressed = 2; break;
							case I_B3: bitPressed = 3; break;
							case I_B4: bitPressed = 4; break;
							case I_B5: bitPressed = 5; break;
							case I_B6: bitPressed = 6; break;
							case I_B7: bitPressed = 7; break;
						}

						if (buttonPressed[I_STOP]) {
							ledBrightness = ((bitPressed + 1) * 4096 / 8) - 1;
							updateLeds();
						} else { // Input

							if (mode == RUN)
								dataButtonPressed = true; // Used to reduce LEDs brightness
							else
								mode = DATA_INPUT;
	
							*INPUT_REG |= 1 << bitPressed;

						}
					break;

					case I_CLEAR:
						if (buttonPressed[I_STOP]) {
							clearRam();
							mode = DATA_INPUT;
							updateLeds();
							blinkLed(L_INPUT, 5);
						} else if (mode == RUN) {
							dataButtonPressed = true;
						} else {
							mode = DATA_INPUT;
						}

						*INPUT_REG = 0;
					break;

					case I_DISPLAY_ADDRESS:
						#ifdef DEBUG
							if (buttonPressed[I_STOP]) {
								uint16_t i;
								for (i = 0; i < 256; i++) {
									Serial.print("0");
									if (*(ram + i) <= 0077) Serial.print("0");
									if (*(ram + i) <= 0007) Serial.print("0");

									Serial.print(*(ram + i), OCT);
									Serial.print(",\n");
								}
							} else
						#endif
							
						if (mode != RUN) {
							mode = ADDRESS;
						}
					break;

					case I_SET_ADDRESS:
						if (mode != RUN) {
							mode = DATA_INPUT;
							addressReg = *INPUT_REG;
						}
					break;

					case I_READ_MEMORY:
						if (buttonPressed[I_STOP]) { // Load RAM

							uint8_t program = *INPUT_REG;

							clearRam();

							// First 4 programs are from the EEPROM
							if (program <= 3) { // from EEPROM
								uint16_t startAddress = program * 256;
								uint16_t i;

								for (i = 0; i < 256; i++)
									*(ram + i) = EEPROM.read(startAddress + i);
								
							} else { // from Flash

								uint16_t i;
								program -= 4;

								for (i = 0; i < sampleProgramsSizes[program]; i++)
									*(ram + i) = samplePrograms[program][i];

							}

							mode = DATA_INPUT;
							addressReg = 0;
							updateLeds();
							blinkLed(L_MEMORY, 5);

						} else if (mode != RUN) {
							mode = MEMORY;
							addressReg++;
						}
					break;

					case I_STORE_MEMORY:
						if (buttonPressed[I_STOP] && *INPUT_REG <= 3) { // Store RAM in EEPROM

							uint16_t startAddress = *INPUT_REG * 256;
							uint16_t i;

							for (i = 0; i < 256; i++)
								EEPROM.update(startAddress + i, *(ram + i));

							blinkLed(L_MEMORY, 5);

						} else if (mode != RUN && digitalRead(I_MEMORY_LOCK)) {
							mode = DATA_INPUT;
							*(ram + addressReg) = *INPUT_REG;
							addressReg++;
						}
					break;

					case I_START:
						if (buttonPressed[I_STOP]) { // Step by step execution
							mode = HALT;
							exec();
						} else {
							mode = RUN;
						}
					break;

					case I_STOP:
						if (mode == RUN)
							mode = HALT;
					break;
				}
			}
		} else {
			buttonPressed[button] = false;
		}
	}

	if (bitPressed == 0xFF)
		dataButtonPressed = false;
}
