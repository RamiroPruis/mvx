\\ASM DATA=300 EXTRA=300 STACK=300
;Pedimos las palabras por teclado
MAIN:   MOV AX, %800
        MOV DX, 10
        MOV CX, 20
        SYS 3

        MOV AX, %800
        MOV DX, 30
        MOV CX, 20
        SYS 3
;Las mostramos antes
        MOV DX, 10
        MOV CX, 20
        SYS 4

        MOV DX, 30
        MOV CX, 20
        SYS 4
;Llamamos a la funcion
        PUSH 10
        PUSH 30
        CALL STRCAT
        ADD SP, 2
;Mostramos la nueva palabra
        MOV DX, 10
        MOV CX, 20
        SYS 4
        STOP

STRCAT: PUSH BP
        MOV BP,SP
        MOV EX, [BP+3] ;Movemos a EX la direccion de memoria de la primer letra de cad1
        MOV FX, [BP+2] ;Movemos a FX la direccion de memoria de la primer letra de cad2
;Primero debemos posicionarnos en el final de cad1
CAD1:   CMP [EX], 0
        JZ CAD2
        ADD EX,1
        JMP CAD1
CAD2:   MOV [EX], [FX]
        CMP [FX], 0
        JZ FIN
        ADD EX, 1
        ADD FX, 1
        JMP CAD2
;Volvemos al main
FIN:    MOV SP,BP
        POP BP
        RET





