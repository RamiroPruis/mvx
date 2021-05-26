//ASM
CAD1 EQU 3
CAD2 EQU 2
;Cargamos las dos palabras
MAIN:   MOV AX,%0 ;Leemos caracter a caracter y va guardando en celdas proximas
        MOV DX, 10 ;Que comience a guardar en la celda 10
        MOV CX, 10 ;Que lea como maximo 10 caracteres
        SYS 3

        MOV AX,%0 
        MOV DX, 20 ;La segunda palabra comienza en la linea 20 
        MOV CX, 10 
        SYS 3
;Mostramos las dos palabras antes del smov
        MOV DX,10
        MOV CX,10
        MOV AX,%800
        SYS %4

        MOV DX,20
        MOV CX,10
        MOV AX,%800
        SYS %4
;LLamamos a la funcion cad1 = cad2
        PUSH 10 ;Lo que estamos pusheando aca son direcciones de memoria, no contenido
        PUSH 20
        CALL SMOV
        ADD SP,2
;Mostramos los resultados
        MOV DX,20
        MOV CX,10
        MOV AX,%800
        SYS %4
        MOV DX,20
        MOV CX,10
        MOV AX,%800
        SYS %4
        STOP

SMOV:   PUSH BP
        MOV BP, SP
        MOV EX, [BP+CAD1] ;Guardamos la direccion de la letra de cad1 en EX
        MOV FX, [BP+CAD2] ;Guardamos la direccion de la letra de cad2 en FX
        CMP [FX], 0 ;Comparamos que la letra de cad2 actual no sea null (para mi aca se rompe)
        JNZ SIGUE
        MOV [FX], 0
        JMP FIN

SIGUE:  MOV [EX], [FX] ;Hacemos la asignacion de letras
        ADD EX,1       ;Sumamos 1 en cada direccion para posterior pusheada
        ADD FX,1
        PUSH EX
        PUSH FX
        CALL SMOV
        JMP FIN

FIN:    MOV SP, BP
        POP BP
        RET