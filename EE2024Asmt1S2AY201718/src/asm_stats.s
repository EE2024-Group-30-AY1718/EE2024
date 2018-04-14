 	.syntax unified
 	.cpu cortex-m3
 	.thumb
 	.align 2
 	.global	asm_stats
 	.thumb_func

asm_stats:
@  Write assembly language code here to compute the statistical parameters
// R1: count, R2: sum, R3: mean, R4: temp, R5: variance, R6: min, R7: max, R8: address shift, R9: address move4

	N: .word 198

	PUSH {R0-R12}

	LDR R1,N
	SUB R1,1
	LDR R2,[R0],#4
	MOV R6,R2
	MOV R7,R2

meansum:
	LDR R3,[R0],#4
	CMP R6,R3 //min
	IT PL
	MOVPL R6,R3
	CMP R7,R3 //max
	IT MI
	MOVMI R7,R3
	ADD R2, R3
	SUBS R1, 1
	BNE meansum

	POP {R0}
	PUSH {R0}
	LDR R1,N

	SDIV R3,R2,R1
	LDR R5, =0

varsum:
	LDR R4,[R0],#4
	SUB R4,R3
	MUL R4,R4
	ADD R5,R4
	SUBS R1,1
	BNE varsum

	LDR R8,=4
	LDR R1,N
	MUL R8,R1
	SUB R1,1
	SDIV R5,R1

	POP {R0}
	LDR R9,=4
	STR R3,[R0,R8]
	ADD R8,R9
	STR R5,[R0,R8]
	ADD R8,R9
	STR R6,[R0,R8]
	ADD R8,R9
	STR R7,[R0,R8]

	POP {R1-R12}

 	BX	LR
