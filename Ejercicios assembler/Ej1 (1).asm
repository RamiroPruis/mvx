\\ASM DATA=2 EXTRA=30 STACK=500

prueba equ 10

MAIN:	PUSH 5 ; valor de la base
	PUSH 2 ; valor del exponente
	CALL POT
	ADD SP , 2
	
	STOP

POT:	PUSH BP
	MOV BP,SP
	PUSH BX
	PUSH CX
	MOV BX,[BP+2] ;Le asigna el exponente a BX
	CMP BX,0  ; if (e == 0) 
	JP SIGOP
	JN SIGON
	MOV AX,1
	JMP FIN

;caso exponente positivo
SIGOP:	SUB BX,1
	PUSH [BP+3] ; La base siempre es la misma, por eso siempre pusheamos ese valor 
	PUSH BX
	CALL POT
	ADD SP,2
	MUL AX,[BP+3]
	JMP FIN

;caso exponente negativo
SIGON: 	ADD BX,1
	PUSH [BP+3] ; llamada recursiva
	PUSH BX
	CALL POT
	ADD SP,2
	MOV CX,1
	DIV CX,[BP+3]
	MUL AX,CX
	JMP FIN
		
FIN:	POP CX
	POP BX
	MOV SP,BP
	POP BP
	RET


	