#include "batteryMeter.h"
#include "config.h"
#include "cpu.h"
#include "inputs.h"
#include "leds.h"
#include "modes.h"

void setup() {

	#ifdef DEBUG
		Serial.begin(SERIAL_BAUD);
	#endif

	ledsInit();
	inputsInit();
	batteryMeterInit();
	modesInit();
	cpuInit();

	readyAnimation();

	#ifdef DEBUG
		Serial.println("Ready");
	#endif
}

void loop() {
	if (mode == RUN) {
		if (exec() == false) {
			mode = HALT;
		} else {
			delay(1000 / CPU_FREQ); // That could be better using millis() and everything
		}
	}

	handleInputs();
	
	updateLeds();
}

