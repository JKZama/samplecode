;Jesse Zamazanuk
;8051 Fibonacci Sequence
;Assume there are 4 switches connected to the lower 4 bits of P1 for a user to be able to enter any
;combination desired up to 15(dec). Assume that if the user enters 0x00, then the program
;does nothing and waits for a proper input.
; If P2.1 is set to '1', then the Fibonacci sequence will be read from the ROM.
; If P2.1 is set to ‘0’, then the Fibonacci sequence will be calculated.
; The result is displayed on P3 with a 27.25 millisecond delay between numbers.
; This code is written to be run on a standard 8051 with 11.0592 MHz frequency.

ORG 200H					;starting location of ROM allocation
DB 0,1,1,2,3,5,8,13,21,34,55,89,144,233,377	;first 15 of fibonacci sequence
READROM BIT P2.1				;bit directive for control bit
		ORG 0
INZERO: 	MOV A, #0FH			;A=0000 1111
		ANL P1, A			;P1 AND A effectively removes upper nibble of P1
		MOV A, P1			;value stored in A				
		JZ INZERO			;if value read was zero, read P1 again
		MOV R5, A			;if value was not zero, value can be moved to R5
		JNB READROM, CALC		;if READROM(P2.1) is zero, the sequence needs to be 
						;calculated
ROM:		MOV A, #1			;need to set A to 1 so that @A+DPTR is the correct location
						;if A is cleared, the first value of the sequence (0) will 
						;be read
						;and JZ will be true, exiting the program
		MOV DPTR, #200H			;starting location of sequence in ROM +1
		MOVC A, @A+DPTR			;moves term of sequence into A
		JZ EXIT				;if a zero is read, the sequence is over and needs to exit
		CJNE A, #121, NCY1		;the value 377 cannot be displayed in 8 bits. It will be 
						;truncated to 79H
		SETB C				;if this is the case, carry bit needs to be set
		SJMP SKIP1			;the clear carry bit needs to be skipped if the value has 
						;been truncated
NCY1:		CLR C				;carry bit is cleared because value was not the truncated 
						;version of 377D
SKIP1:		MOV P3, #0			;set P3 to output
		MOV P3, A			;output value
		INC DPTR			;point to location of next value
		ACALL DELAY			;delay so that value is not displayed twice
		DJNZ R5, ROM			;do this however many times the user specified
		SJMP EXIT			;display of sequence is finished

CALC:		MOV A, #1 			;needs 1 to start calculating sequence			
		MOV P3, 0			;make p3 output port (first value of sequence does not need 
						;to be displayed
						;because it is zero
		ACALL DELAY
		DEC R5				;however, zero is still the first number of the sequence, so 
						;dec r5
		MOV P3, A			
		ACALL DELAY
		DEC R5
SEQ:		MOV R1, A			
		ADD A, R0
		CJNE A, #122, NCY2		
		SETB C
		DEC A
		SJMP SKIP2
NCY2:		CLR C
SKIP2:		MOV P3, #0
		MOV P3, A
		MOV 0, 1
		ACALL DELAY
		DJNZ R3, SEQ
EXIT:
DELAY: 	MOV R2, #155
HERE1:	MOV R3, #79
HERE2:	DJNZ R3, HERE2
		NOP
		DJNZ R2, HERE1
		NOP
		NOP
		RET
		END
