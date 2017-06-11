Kenbak-1 Replica
================
Replica of the [Kenbak-1](http://kenbak-1.net): the first personnal computer.

All the internal TTL logic has been replaced by an Arduino Nano.
An MCP23017 handle the inputs.
An TLC5940NT handle the LEDs.

![Schematic](../assets/Schematic.png)

Differences with the original
-----------------------------
* Useful shortcuts (see below).
* Memory initialized to 0.
* Unprecise timing (original Kenbak-1 instructions had an unknown number of cycles).

Shortcuts
---------
* STOP + START: As the original Kenbak-1, executes one instruction.
* STOP + READ: Load RAM n°X. X is determined by the input number entered on the Kenbak-1 (input register: memory address 0377). Programs 0-3 come from the Arduino's EEPROM, others from the Arduino's Flash ([sample programs](src/samplePrograms.cpp)).
* STOP + STORE: Store RAM in EEPROM bank n°X. Only numbers 0-3 are allowed.
* STOP + CLEAR: Clear all RAM (initialized to 0).
* STOP + DISPLAY: Dump RAM to serial if `DEBUG` is defined (config.h)[src/config.h] in a C-array friendly manner. I use this so I can enter a program from the Kenbak-1 and then save it as a [sample program](src/samplePrograms.cpp).
* STOP + B0-B7: Change LEDs' brightness.

Sample programs
---------------
4. Automatic counter (*[Laboratory Exercises][LabExer]* Exercise 2, page 2-3 and almost Exercise 12, page 12-5)
5. Step-by-step counter (*[Laboratory Exercises][LabExer]* Exercise 3, page 3-1)
6. Binary to decimal training (*[Laboratory Exercises][LabExer]* Exercise 4, page 4-3)
7. Decimal to binary training (*[Laboratory Exercises][LabExer]* Exercise 5, page 5-1)
8. Adder (*[Laboratory Exercises][LabExer]* Exercise 6, page 6-1)
9. Combination lock, code is 42 (*[Laboratory Exercises][LabExer]* Exercise 11, page 11-7)
10. Separate tally (*[Laboratory Exercises][LabExer]* Exercise 13, page 13-4)
. K-2000 animation
. Multiplier: push START to start, enter first term, push START, enter second term, push START, results displays, push START to start again.

TODO
----
* Use MCP23017 interrupts.
* Make loading programs works with unconsecutive bit buttons and not starting at pin 8.
* Use internal timer to check battery's voltage every X sec.
* Make the `bitPressed` variable assignment better in `handleInputs()`.
* Uniformize `switch`s and `if`s in `exec()`.
* Check the `delay(50)` in `inputsInit()`.
* Make function to print the operand while debugging.
* Make it so that when you press STORE to store a program to the EEPROM, it doesn't store in Kenbak-1's memory.

[LabExer]: http://www.kenbak-1.net/index_files/Lab%20Exer.pdf

Quirks
------
* *[Laboratory Exercises][LabExer]* page 13-4: instruction `STORE A (200)` should be `STORE A OUTPUT`.