Multiplier
==========

Multiply two numbers.

1. Push START
2. Enter first number
3. Push START
4. Enter second number
5. Push START
6. …
7. Profit! Calculation appears on data lights
8. Go back to 1. to start again

```
LOC	DATA	Symbolic Address	Content
000			A
001			B
002			X
003	004		P					START
200			OUTPUT
377			INPUT

004	223		START				LOAD X C=0
005	000
006	234							STORE X OUTPUT
007	200
010	234							STORE X INPUT
011	377
012	024							LOAD A X
013	002
014	000							HALT
015	124							LOAD B INPUT
016	377
017	234							STORE X INPUT
020	377
021	000							HALT
022	224							LOAD X INPUT
023	377
024	244							JPD X=0 START
025	004
026	004		LOOP				ADD A B
027	001
030	213							SUB X C=1
031	001
032	243							JPD X≠0 LOOP
033	024
034	034		STOP				STORE A OUTPUT
035	200
036	000							HALT
037	344							JPD UNC START
040	004
```