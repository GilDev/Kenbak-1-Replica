#ifndef INPUTS_H
#define INPUTS_H

#include <Wire.h>
#include <Adafruit_MCP23017.h>

extern Adafruit_MCP23017 inputs;

/**
 * Gives the state of the inputs.
 */
extern bool buttonPressed[16];

/**
 * True if a bit button is pressed.
 */
extern bool dataButtonPressed;

/**
 * Initialize inputs handler.
 */
void inputsInit(void);

/**
 * Handle inputs.
 */
void handleInputs(void);

#endif
