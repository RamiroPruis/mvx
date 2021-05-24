\\ASM

	;espacio 1
	mov CX , 20
	SYS 5
	;espacio 2
	MOV CX , 40
	sys 5
	MOV AC, DX
	;espacio 3
	mov CX , 30
	SYS 5
	;espacio 4
	MOV CX , 20
	sys 5
	MOV AX , DX
	
	;libero espacio 2
	MOV DX , AC
	SYS 6
	;libero espacio 4
	MOV DX , AX
	SYS 6
	STOP
	
	
	