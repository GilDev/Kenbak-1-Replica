#ifndef LEDS_H
#define LEDS_H

#include <Tlc5940.h>

extern Tlc5940 tlc;

/**
 * List of LEDs in order.
 *
 * From L_B7 to L_RUN (left to right).
 * Used to play the "ready animation" and for blinking when loading program.
 */
extern const uint8_t leds[];

/**
 * LEDs brightness levels (12-bit value).
 */
extern uint16_t ledBrightness, ledLowBrightness;

/**
 * Initialize LEDs.
 */
void ledsInit(void);

/**
 * Play a back and forth animation with the LEDs.
 */
void readyAnimation(void);

/**
 * Blink an LED.
 * \param[in] ledPin Led to blink.
 * \param[in] number Number of blinks.
 */
void blinkLed(uint8_t ledPin, uint8_t number);

/**
 * Update LEDs based on current mode.
 */
void updateLeds(void);

#endif
