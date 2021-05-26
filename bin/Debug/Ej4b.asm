\\ASM
division EQU -1
numero EQU 3
MAIN:       MOV AX, %001                    ;Lo lee como decimal
            MOV DX, 0
            MOV CX, 1
            SYS 1

            SYS 2                           ;Lo mostramos por las dudas

            PUSH [DX]                       ;Pusheamos el numero int
            MOV DX, 10                      ;Cambio el valor del DX porque en la celda 10 quiero comenzar a guardar el numero
            PUSH AC                         ;Pusheamos AC porque lo vamos a necesitar en la recursion
            CALL INTSTR
            ADD SP, 2
            
            MOV AX, %010
            MOV DX, 10
            MOV CX, 10
            SYS 2
            STOP

INTSTR:     PUSH BP
            MOV BP,SP
            SUB SP, 1                       ;Reservamos un espacio de memoria para la varaible local que almacenara la division truncada
            MOV [BP+division], [BP+numero]  ;Pasamos el valor del numero a la variable local
            DIV [BP+division], 10           ;Dividimos y guardamos en la local el resultado, en AC se guarda el resto
            CMP [BP+division], 0
            JNZ SIGUE
            ADD AC, '0'
            MOV [DX], AC
            ADD DX, 1
            JMP FIN

SIGUE:      PUSH [BP+division]              ;Pusheamos el numero
            PUSH AC
            CALL INTSTR
            POP AC
            ADD AC, '0'
            MOV [DX], AC
            ADD DX, 1
            JMP FIN

FIN:        MOV SP,BP
            POP BP
            RET




                