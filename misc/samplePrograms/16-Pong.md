Pong
====

Really simple 1 dimension Pong game.

Push start to begin. When the ball is at bit 7, push bit 7 button at the same time to throw the ball back to the right.

You can use other bit buttons if you like, it may increase difficulty.

```
LOC	DATA	Symbolic Address	Content
000			A	
001			B	
002			X	
003	004		P					START
200			OUTPUT	
377			INPUT	
				
004	023		START				LOAD A C=1
005	001			
006	311		LEFT				ROTL A1
007	034							STORE A OUTPUT
010	200			
011	124							LOAD B A
012	000			
013	114							SUB B INPUT
014	337			
015	234							STORE X INPUT
016	377			
017	143							JPD B≠0 LEFT
020	006			
021	011		RIGHT				SFTR A1
022	034							STORE A OUTPUT
023	200			
024	124							LOAD B A
025	000			
026	113							SUB B C=1
027	001			
030	143							JPD B≠0 RIGHT
031	021			
032	344							JPD UNC LEFT
033	006			
```