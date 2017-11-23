; Jesse Zamazanuk
; Simple binary counter
; This program is written for the Philips P89LPC932A1 Simon development board with LEDs on
;  the following:
; Port (significance of binary digit (higher is more significant))
; P2.4(3)
; P0.5(2)
; P2.7(1)
; P0.6(0)
; Counter is initially at zero, and count up or down can be done by pressing the
;  push-button switch on P2.0 or P0.2 respectively.
; When the count rolls over (15 to 0 counting up or 0 to 15 counting down), a C6 square
;  wave (approx 1046.50 Hz) is sent to the speaker (P1.7)


#include<reg932.inc> ;includes port definitions for P89LPC932A1

		CSEG AT 0

		MOV P0M1, #0		;Setting all P0 ports to bi-directional
		MOV P1M1, #0		;Setting all P1 ports to bi-directional
		MOV P2M1, #0		;Setting all P2 ports to bi-directional
		MOV R0, #0		;Initializing counter to 0
START:		MOV C, P2.0		;Checking to see if SW1 is pressed
		JC START2		;Jumping to START2 if SW1 is not pressed
		SJMP CHECK1		;Jumping to CHECK1 if SW1 is pressed
START2:		MOV C, P0.2		;Checking to see if SW2 is pressed
		JC START		;Jumping back to START if S2 is not pressed
	     	SJMP CHECK2		;Jumping to CHECK2 if SW2 is pressed


CHECK1:		ACALL DELAYREAD		;Calling the DELAYREAD subroutine
		MOV C, P2.0		;Checking to see if SW1 is still pressed
		JNC START		;If SW1 is not still pressed, jump back to START
		SJMP COUNTUP1		;If SW1 is still pressed, jump to COUNTUP1

CHECK2:		ACALL DELAYREAD		;Calling the DELAYREAD subroutine
		MOV C, P0.2		;CHECKING to see if SW2 is still pressed
		JNC START		;If SW2 is not still pressed, jump back to START
		LJMP COUNTD1		;IF SW2 is still pressed, jump to COUNTD1

COUNTUP1:	CJNE R0, #15, COUNTUP2 	;Check to see if the counter is at 15, if not, jump to COUNTUP2
        	ACALL PLAYC6		;Call to PLAYC6 subroutine
		MOV R0, #0		;Set the counter to 0
		MOV R1, #3		;Counter for LED UPSEQ loop
		SETB P2.4		;Setting the bits for all four counting LEDS (turns all off to display zero)
		SETB P0.5
		SETB P2.7
		SETB P0.6

UPSEQ:                  ;this is the LED animation for when counting up from 15 (count rolls over)
		CLR P2.5		;Light bottom row of leds
		CLR P0.7
		CLR P2.6
		ACALL DELAYLED
		SETB P2.5       ;turn off bottom row and turn on second row
		SETB P0.7
		SETB P2.6
		CLR P0.6
		CLR P1.6
		CLR P0.4
		ACALL DELAYLED
		SETB P0.6       ;turn off second row and turn on top row
		SETB P1.6
		SETB P0.4
		CLR P2.4
		CLR P0.5
		CLR P2.7
		ACALL DELAYLED
		SETB P2.4       ;turn off top row (All LEDs are now off)
		SETB P0.5
		SETB P2.7
	
		DJNZ R1, UPSEQ

		LJMP START
		
COUNTUP2:	INC R0			;Increment the counter
		CJNE R0, #1, UNOT1	;Check if the counter is 1, if not, jump to UNOT1
		CLR P0.6		;Clearing the bit for P0.6
        	ACALL PLAYC5		;Calling the subroutine to play C5
		LJMP START		;Jumping back to START

UNOT1:		CJNE R0, #2, UNOT2	;Check if the counter is 2, if not, jump to UNOT2
		CLR P2.7		;Clearing the bit for P2.7
		SETB P0.6		;Setting the bit for P0.6
		ACALL PLAYD5		;Calling the subroutine to play D5
	       	LJMP START

UNOT2:		CJNE R0, #3, UNOT3	;Check if the counter is 3, if not, jump forward
		CLR P0.6		;Clearing the bit for P0.6
		ACALL PLAYE5		;Calling subroutine to play E5
		LJMP START

UNOT3:		CJNE R0, #4, UNOT4	;Checking if counter is 4, jumping if not, and setting 
		CLR P0.5		;	the counting LEDs correctly, and playing the
		SETB P0.6		;	next note of the scale
		SETB P2.7
		ACALL PLAYF5
		LJMP START

UNOT4:		CJNE R0, #5, UNOT5	;Checking if the counter is 5, jumping if not
		CLR P0.6		;Setting the LEDs to show 5, and playing G5
		ACALL PLAYG5
		LJMP START

UNOT5:		CJNE R0, #6, UNOT6	;Checking if the counter is 6, jumping if not
		CLR P2.7		;Setting the LEDs to show 6, and playing A5
		SETB P0.6
		ACALL PLAYA5
		LJMP START

UNOT6:		CJNE R0, #7, UNOT7	;Checking if the counter is 7, jumping if not
		CLR P0.6		;Setting the LEDs to show 7, and playing B5
		ACALL PLAYB5
		LJMP START

UNOT7:		CJNE R0, #8, UNOT8	;Checking if the counter is 8, jumping if not
		CLR P2.4		;Setting the LEDs to show 8, and playing C6
		SETB P0.5
		SETB P2.7
		SETB P0.6
		ACALL PLAYC6
		LJMP START

UNOT8:		CJNE R0, #9, UNOT9	;Checking if the counter is 9, jumping if not
		CLR P0.6		;Setting the LEDs to show 9, and restarting the
		ACALL PLAYC5		;	scale by playing C5
		LJMP START


UNOT9:		CJNE R0, #10, UNOT10	;Checking if the counter is 10, jumping if not
		CLR P2.7		;Setting the LEDs to show 10, and playing D5
		SETB P0.6
		ACALL PLAYD5
		LJMP START

UNOT10:		CJNE R0, #11, UNOT11	;Checking if the counter is 11, jumping if not
		CLR P0.6		;Setting the LEDs to show 11, and playing E5
		ACALL PLAYE5
		LJMP START

UNOT11:		CJNE R0, #12, UNOT12	;Checking if the counter is 12, jumping if not
		CLR P0.5		;Setting the LEDs to show 12, and playing F5
		SETB P0.6
		SETB P2.7
		ACALL PLAYF5
		LJMP START

UNOT12:		CJNE R0, #13, UNOT13	;Checking if the counter is 13, jumping if not
		CLR P0.6		;Setting the LEDs to show 13, and playing G5
		ACALL PLAYG5
		LJMP START

UNOT13:		CJNE R0, #14, UNOT14	;Checking if the counter is 14, jumping if not
		CLR P2.7		;Setting the LEDs to show 14, and playing A5
		SETB P0.6
		ACALL PLAYA5
		LJMP START

UNOT14:		CLR P0.6		;Setting the LEDs to show 15, playing B5, and
       		ACALL PLAYB5		;	jumping back to START
		LJMP START

COUNTD1:	CJNE R0, #0, DNOT15	;Checking if the counter is at 0, jumping if it's not
        	ACALL PLAYC6		;Call to subroutine to play C6
		MOV R0, #15		;Setting the counter to 15
		MOV R1, #3		;counter for LED DSEQ

DSEQ:	
		CLR P2.4		;LED animation opposite of UPSEQ
		CLR P0.5
		CLR P2.7
		ACALL DELAYLED
		SETB P2.4
		SETB P0.5
		SETB P2.7
		CLR P0.6
		CLR P1.6
		CLR P0.4
		ACALL DELAYLED
		SETB P0.6
		SETB P1.6
		SETB P0.4
		CLR P2.5
		CLR P0.7
		CLR P2.6
		ACALL DELAYLED
		SETB P2.5
		SETB P0.7
		SETB P2.6
		DJNZ R1, DSEQ
		CLR P2.4
		CLR P2.7
		CLR P0.5
		CLR P0.6
	
		LJMP START

DNOT15:		DEC R0			;Decrementing the counter
		CJNE R0, #14, DNOT14	;Checking if the counter is 14, jumping if not
		SETB P0.6		;Setting the LEDs to 14, and playing B5
		ACALL PLAYB5
		LJMP START


DNOT14:		CJNE R0, #13, DNOT13	;Checking if the counter is 13, jumping if not
		SETB P2.7		;Setting the LEDs to 13, and playing A5
		CLR P0.6
		ACALL PLAYA5
		LJMP START

DNOT13:		CJNE R0, #12, DNOT12	;Checking if the counter is 12, jumping if not
		SETB P0.6		;Setting the LEDs to 12, and playing G5
		ACALL PLAYG5
	       	LJMP START

DNOT12:		CJNE R0, #11, DNOT11	;Checking if the counter is 11, jumping if not
		SETB P0.5		;Setting the LEDs to 11, and playing F5
		CLR P2.7
		CLR P0.6
		ACALL PLAYF5
		LJMP START

DNOT11:		CJNE R0, #10, DNOT10	;Checking if the counter is 10, jumping if not
		SETB P0.6		;Setting the LEDs to 10, and playing E5
		ACALL PLAYE5
		LJMP START

DNOT10:		CJNE R0, #9, DNOT9	;Checking if the counter is 9, jumping if not
		CLR P0.6		;Setting the LEDs to 9, playing D5
		SETB P2.7
		ACALL PLAYD5
		LJMP START

DNOT9:		CJNE R0, #8, DNOT8	;Checking if the counter is 8, jumping if not
		SETB P0.6		;Setting the LEDs to 8, playing C5
		ACALL PLAYC5
		LJMP START

DNOT8:		CJNE R0, #7, DNOT7	;Checking if the counter is 7, jumping if not
		CLR P0.6		;Setting the LEDs to 7, playing C6
		CLR P2.7
		CLR P0.5
		SETB P2.4
		ACALL PLAYC6
		LJMP START

DNOT7:		CJNE R0, #6, DNOT6	;Checking if the counter is 6, jumping if not
		SETB P0.6		;Setting the LEDs to 6, playing B5
		ACALL PLAYB5
		LJMP START

DNOT6:		CJNE R0, #5, DNOT5	;Checking if the counter is 5, jumping if not
		SETB P2.7		;Setting the LEDs to 5, playing A5
		CLR P0.6
		ACALL PLAYA5
		LJMP START


DNOT5:		CJNE R0, #4, DNOT4	;Checking if the counter is 4, jumping if not
		SETB P0.6		;Setting the LEDs to 4, playing G5
		ACALL PLAYG5
		LJMP START

DNOT4:		CJNE R0, #3, DNOT3	;Checking if the counter is 3, jumping if not
		CLR P0.6		;Setting the LEDs to 3, playing F5
		CLR P2.7
		SETB P0.5
		ACALL PLAYF5
		LJMP START

DNOT3:		CJNE R0, #2, DNOT2	;Checking if the counter is 2, jumping if not
		SETB P0.6		;Setting the LEDs to 2, playing E5
		ACALL PLAYE5
		LJMP START

DNOT2:		CJNE R0, #1, DNOT1	;Checking if the counter is 1, jumping if not
		CLR P0.6		;Setting the LEDs to 1, playing D5
		SETB P2.7
		ACALL PLAYD5
		LJMP START

DNOT1:		SETB P0.6		;Setting the LEDs to 0, playing C5, jumping back to START
        	ACALL PLAYC5
		LJMP START

DELAYREAD:	MOV R7, #50		;Delay subroutine to check if the button is still
LOOP3:		MOV R6, #50		;	being pressed, and to avoid having the button
LOOP2:		MOV R5, #200		;	pressed cause multiple inputs
LOOP1:		NOP
		NOP
		DJNZ R5, LOOP1
		DJNZ R6, LOOP2
		DJNZ R7, LOOP3
		RET

DELAYLED:	MOV R7, #12		;Delay subroutine for flashing LEDs = about 250ms
LOOP4:		MOV R6, #250
LOOP5:		MOV R5, #250
LOOP6:		DJNZ R5, LOOP6
		DJNZ R6, LOOP5
		DJNZ R7, LOOP4
		RET
		

		
PLAYC5:	   	MOV R4, #200		;Subroutine to determine the length of C5
C5LOOP1:	CPL P1.7		;	that also calls the subroutine for the
         	ACALL C5DELAY		;	pitch
          	CPL P1.7
          	ACALL C5DELAY
		DJNZ R4, C5LOOP1
		RET
	
C5DELAY:    	MOV R7, #50		;Sets the delay between compliments for P1.7
C5LOOP4:   	MOV R6, #141		;	determining the frequency of the pitch
C5LOOP3:   	DJNZ R6, C5LOOP3
           	DJNZ R7, C5LOOP4
           	RET

PLAYD5:		MOV R4, #200
D5LOOP1:	CPL P1.7
	        ACALL D5DELAY
       	 	CPL P1.7
       	 	ACALL D5DELAY
		DJNZ R4, D5LOOP1
		RET

D5DELAY:   	MOV R7, #50
D5LOOP3:   	MOV R6, #125
D5LOOP2:   	DJNZ R6, D5LOOP2
           	DJNZ R7, D5LOOP3
           	RET
            
PLAYE5:		MOV R4, #200
E5LOOP1:	CPL P1.7
        	ACALL E5DELAY
        	CPL P1.7
       		ACALL E5DELAY
		DJNZ R4, E5LOOP1
		RET
		
E5DELAY:    	MOV R7, #50
E5LOOP3:    	MOV R6, #111
E5LOOP2:    	DJNZ R6, E5LOOP2
            	DJNZ R7, E5LOOP3
            	RET		
		
PLAYF5:		MOV R4, #200
F5LOOP1:	CPL P1.7
        	ACALL F5DELAY
        	CPL P1.7
        	ACALL F5DELAY
		DJNZ R4, F5LOOP1
		RET

F5DELAY:    	MOV R7, #50
F5LOOP3:    	MOV R6, #105
F5LOOP2:   	DJNZ R6, F5LOOP2
            	DJNZ R7, F5LOOP3
            	RET
            
PLAYG5:		MOV R4, #200
G5LOOP1:	CPL P1.7
        	ACALL G5DELAY
        	CPL P1.7
        	ACALL G5DELAY
		DJNZ R4, G5LOOP1
		RET

G5DELAY:    	MOV R7, #50
G5LOOP3:    	MOV R6, #94
G5LOOP2:    	DJNZ R6, G5LOOP2
            	DJNZ R7, G5LOOP3
            	RET
            
PLAYA5:		MOV R4, #200
A5LOOP1:	CPL P1.7
        	ACALL A5DELAY
        	CPL P1.7
        	ACALL A5DELAY
		DJNZ R4, A5LOOP1
		RET

A5DELAY:    	MOV R7, #50
A5LOOP3:    	MOV R6, #83
A5LOOP2:    	DJNZ R6, A5LOOP2
            	DJNZ R7, A5LOOP3
            	RET
            
PLAYB5:		MOV R4, #200
B5LOOP1:	CPL P1.7
        	ACALL B5DELAY
        	CPL P1.7
        	ACALL B5DELAY
		DJNZ R4, B5LOOP1
		RET

B5DELAY:    	MOV R7, #50
B5LOOP3:    	MOV R6, #74
B5LOOP2:    	DJNZ R6, B5LOOP2
            	DJNZ R7, B5LOOP3
            	RET
            
PLAYC6:		MOV R4, #200
C6LOOP1:	CPL P1.7
        	ACALL C6DELAY
        	CPL P1.7
        	ACALL C6DELAY
		DJNZ R4, C6LOOP1
		RET
		
C6DELAY:    	MOV R7, #50
C6LOOP3:    	MOV R6, #70
C6LOOP2:    	DJNZ R6, C6LOOP2
            	DJNZ R7, C6LOOP3
            	RET	
            
            
		END