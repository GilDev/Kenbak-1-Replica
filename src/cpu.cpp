#include <Arduino.h>
#include "config.h"
#include "cpu.h"

enum AddressingModes {
	CONSTANT         = 03,
	MEMORY           = 04,
	INDIRECT         = 05,
	INDEXED          = 06,
	INDIRECT_INDEXED = 07
};

uint8_t ram[256];
uint8_t addressReg;

void cpuInit(void)
{
	addressReg = 0;
	clearRam();
}

void clearRam(void)
{
	uint16_t i;
	for (i = 0; i < 256; i++)
		ram[i] = 0;
}

uint8_t* getOperand(uint8_t *data, uint8_t addressingMode)
{
	switch (addressingMode) {
		case CONSTANT:
			return data;

		case MEMORY:
			return ram + *data;

		case INDIRECT:
			return ram + *(ram + *data);

		case INDEXED:
			return ram + *X_REG + *data;

		case INDIRECT_INDEXED:
			return ram + *X_REG + *(ram + *data);
	}

	return 0;
}

void printOperand(uint8_t *data, uint8_t addressingMode)
{
	Serial.print(" ");

	switch (addressingMode) {
		case CONSTANT:
			Serial.print("C=");
			Serial.print(*data, DEC);
		break;

		case MEMORY:
			Serial.print(*data, OCT);
		break;

		case INDIRECT:
			Serial.print("(");
			Serial.print(*(ram + *data), OCT);
			Serial.print(")");
		break;

		case INDEXED:
			Serial.print(*data, OCT);
			Serial.print(", X");
		break;

		case INDIRECT_INDEXED:
			Serial.print("(");
			Serial.print(*(ram + *data), OCT);
			Serial.print("), X");
		break;
	}
}

bool exec(void)
{
	bool halt = false;

	#ifdef DEBUG
		Serial.print("PC: ");
		Serial.println(*P_REG, OCT);
	#endif

	uint8_t instruction = *(ram + *P_REG);

	#ifdef DEBUG
		Serial.print("Instruction: ");
		Serial.println(instruction, OCT);
	#endif

	// Instruction split
	uint8_t D__ = (instruction & 0300) >> 6;
	uint8_t _D_ = (instruction & 0070) >> 3;
	uint8_t __D = (instruction & 0007) >> 0;

	uint8_t *data = ram + *P_REG + 1;

	#ifdef DEBUG
		Serial.print("Data: ");
		Serial.println(*data, OCT);
	#endif

	if (__D == 0) { // Miscellaneous (one byte only)

		if (D__ == 0 || D__ == 1) { // Halt

			halt = true;

			#ifdef DEBUG
				Serial.print("HALT");
			#endif

		} else { // Noop

			#ifdef DEBUG
				Serial.print("NOOP");
			#endif

		}

		(*P_REG)++;

	} else if (__D == 1) { // Shifts, Rotates (one byte only)

		bool left = D__ & 02;

		bool rotate = D__ & 01;

		uint8_t *reg;
		if (_D_ & 04)
			reg = B_REG;
		else
			reg = A_REG;
		
		uint8_t places = _D_ & 03;
		if (places == 0)
			places = 4;

		uint8_t backup = *reg;
		if (left) {
			*reg <<= places;
			if (rotate)
				*reg |= backup >> (8 - places);
		} else {
			*reg >>= places;
			if (rotate)
				*reg |= backup << (8 - places);
			//else if (backup | 0200) { // Carry the MSB
				//*reg |= ((1 << places) - 1) << 8 - places;
			//}
		}

		#ifdef DEBUG
			if (rotate)
				Serial.print("ROT");
			else
				Serial.print("SFT");

			if (left)
				Serial.print("L ");
			else
				Serial.print("R ");

			if (reg == A_REG)
				Serial.print("A");
			else
				Serial.print("B");

			Serial.print(places);
		#endif

		(*P_REG)++;

	} else if (__D == 2) { // Bit Test and Manipulation

		if (D__ & 02) { // Skip

			bool skip;
			if ((*(ram + *data) & (1 << _D_)) == (D__ & 01))
				skip = true;
			else
				skip = false;

			#ifdef DEBUG
				Serial.print("SKP ");
				Serial.print(D__ & 01);
				Serial.print(" b");
				Serial.print(_D_);
			#endif

			if (skip)
				*P_REG += 2;

		} else { // Set

			if (D__ & 01) // Set 1
				*(ram + *data) |= (1 << _D_);
			else          // Set 0
				*(ram + *data) &= ~(1 << _D_);

			#ifdef DEBUG
				Serial.print("SET ");
				Serial.print(D__ & 01);
				Serial.print(" b");
				Serial.print(_D_);
			#endif

		}

		*P_REG += 2;

	} else if (_D_ >= 4) { // Jumps

		bool mark = _D_ & 02;
		if (_D_ & 01) { // Indirect addressing
			data = ram + *data;
		}

		bool condition;
		if (D__ == 03)      // Unconditional
			condition = true;
		else if (__D == 03) // Non-zero
			condition = *(ram + D__);
		else if (__D == 04) // Zero
			condition = !(*(ram + D__));
		else if (__D == 05) // Negative
			condition = *(ram + D__) & 0200;
		else if (__D == 06) // Positive
			condition = !(*(ram + D__) & 0200);
		else if (__D == 07) // Positive Non-zero
			condition = !(*(ram + D__) & 0200) && (*(ram + D__) & 0177);

		if (condition) {
			if (mark) {
				*(ram + *data) = *P_REG + 2; 
				*P_REG = *data + 1;
			} else {
				*P_REG = *data;
			}
		} else {
			*P_REG += 2;
		}

		#ifdef DEBUG
			Serial.print("J");

			if (mark)
				Serial.print("M");
			else
				Serial.print("P");

			if (_D_ & 01)
				Serial.print("I ");
			else
				Serial.print("D ");

			if (D__ == 03) {
				Serial.print("UNC");
			} else {
				if (D__ == 00)
					Serial.print("A");
				else if (D__ == 01)
					Serial.print("B");
				else if (D__ == 02)
					Serial.print("X");
			
				if (__D == 03)
					Serial.print("!=0");
				else if (__D == 04)
					Serial.print("=0");
				else if (__D == 05)
					Serial.print("<0");
				else if (__D == 06)
					Serial.print(">=0");
				else if (__D == 07)
					Serial.print(">0");

				Serial.print(" ");

				if (mark)
					Serial.print("(");
				Serial.print(*data);
				if (mark)
					Serial.print(")");
			}
		#endif

	} else if (D__ == 03) { // Or, And, Lneg

		uint8_t *operand = getOperand(data, __D);

		switch (_D_) {
			case 2: // And
				*A_REG &= *operand;

				#ifdef DEBUG
					Serial.print("AND");
				#endif

				*P_REG += 2;
			break;

			case 0: // Or
				*A_REG |= *operand;

				#ifdef DEBUG
					Serial.print("OR");
				#endif

				*P_REG += 2;
			break;

			case 3: // Lneg
				*A_REG = (uint8_t) (-(int8_t) *operand);

				#ifdef DEBUG
					Serial.print("LNEG");
				#endif

				*P_REG += 2;
			break;

			case 1: // (Noop)
				#ifdef DEBUG
					Serial.print("NOOP");
				#endif

				(*P_REG)++;
			break;
		}

		printOperand(data, __D);

	} else { // Add, Sub, Load, Store

		uint8_t *operand = getOperand(data, __D);
		uint16_t result;
		int16_t tmp;

		switch (_D_) {
			case 0: // Add
				result = *(ram + D__) + *operand;

				*(ram + D__) = result;

				*(OVERFLOW_AND_CARRY_A_REG + D__) = 0000;

				if (result & 0xFF00) // Carry
					*(OVERFLOW_AND_CARRY_A_REG + D__) |= 0002;

				tmp = (int8_t) *(ram + D__) + (int8_t) *operand;
				if (tmp < -128 || tmp > 127)
					*(OVERFLOW_AND_CARRY_A_REG + D__) |= 0001;

				#ifdef DEBUG
					Serial.print("ADD");
				#endif

			break;

			case 1: // Sub
				result = *(ram + D__) - *operand;
				*(ram + D__) = result;
				*(OVERFLOW_AND_CARRY_A_REG + D__) = 0000;

				if (result & 0xFF00) // Carry
					*(OVERFLOW_AND_CARRY_A_REG + D__) |= 0002;

				tmp = (int8_t) ram[D__] - (int8_t) *operand;
				if (tmp < -128 || tmp > 127)
					*(OVERFLOW_AND_CARRY_A_REG + D__) |= 0001;

				#ifdef DEBUG
					Serial.print("SUB");
				#endif

			break;

			case 2: // Load
				*(ram + D__) = *operand;

				#ifdef DEBUG
					Serial.print("LOAD");
				#endif

			break;

			case 3: // Store
				*operand = *(ram + D__);

				#ifdef DEBUG
					Serial.print("STORE");
				#endif

			break;
		}

		Serial.print(" ");

		if (D__ == 00)
			Serial.print("A");
		else if (D__ == 01)
			Serial.print("B");
		else if (D__ == 02)
			Serial.print("X");

		printOperand(data, __D);

		*P_REG += 2;
	}
		
	Serial.print("\n");

	#ifdef DEBUG
		Serial.print("A = ");
		Serial.print(*A_REG);
		Serial.print("\tB = ");
		Serial.print(*B_REG);
		Serial.print("\tX = ");
		Serial.print(*X_REG);
		Serial.print("\n\n");
	#endif

	if (halt)
		return false;

	return true;
}

