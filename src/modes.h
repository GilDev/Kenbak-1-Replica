#ifndef MODES_H
#define MODES_H

#include <Arduino.h>

/**
 * Kenbak-1's modes.
 */
enum Modes {DATA_INPUT, ADDRESS, MEMORY, RUN, HALT};

/**
 * Current Kenbak-1's mode.
 */
extern uint8_t mode;

/**
 * Initialize Kenbak-1's modes.
 */
void modesInit(void);

#endif
