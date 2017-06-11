#ifndef CPU_H
#define CPU_H

#include <Arduino.h>

/** @{ */
/**
 * CPU's registers.
 */
#define A_REG                    (ram + 00)
#define B_REG                    (ram + 01)
#define X_REG                    (ram + 02)
#define P_REG                    (ram + 03)
#define OUTPUT_REG               (ram + 0200)
#define OVERFLOW_AND_CARRY_A_REG (ram + 0201)
#define OVERFLOW_AND_CARRY_B_REG (ram + 0202)
#define OVERFLOW_AND_CARRY_X_REG (ram + 0203)
#define INPUT_REG                (ram + 0377)
//* @} */

/**
 * Kenbak-1's RAM.
 */
extern uint8_t ram[256];

/**
 * Register pointing to the memory location to edit.
 */
extern uint8_t addressReg;

/**
 * Initialize Kenbak-1's CPU.
 */
void cpuInit(void);

/**
 * Clear Kenbak-1's RAM.
 */
void clearRam(void);

/**
 * Execute the next instruction.
 *
 * \return True if halt instruction is executed.
 */
bool exec(void);

#endif
