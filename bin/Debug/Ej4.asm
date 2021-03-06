\\ASM
MAIN:       MOV AX, %800
            MOV DX, 0
            MOV CX, 30
            SYS 3
            SYS 4
            PUSH DX         ;Pusheamos la direccion
            CALL STRINT
            ADD SP, 1       ;Porque no nos importa la direccion de la palabra
            MOV AX, 1
            MOV DX, FX      ;FX vuelve de la funcion con la direccion de memoria donde se encuentra el num
            MOV CX, 1
            SYS 2           ;Mostramos
            STOP

STRINT:     PUSH BP
            MOV BP,SP
            SUB SP, 3       ;Una guarda el signo, otra la suma, y otra es aux para el pasaje de char a int
            MOV [BP-1], 1   ;Inicializamos en 1 el signo
            MOV [BP-2], 0   ;Inicializamos en 0 la suma
            MOV FX, [BP+2]  ;Fx toma la direccion del char
            CMP [FX], '-'   ;Comparamos con el negativo
            JZ NEGADO
            JMP SIGUE

SIGUE:      CMP [FX], 0
            JZ FIN
            MOV [BP-3], [FX]    ;Pasamos el char
            SUB [BP-3], '0'     ;Lo pasamos a int
            MUL [BP-2], 10      ;A la suma anterior la mult por diez
            ADD [BP-2], [BP-3]  ;Le sumamos el nuevo int
            ADD FX, 1           ;Avanzamos
            JMP SIGUE

NEGADO:     NOT [BP-1]
	    ADD [BP-1],1
            ADD FX,1
            JMP SIGUE

FIN:        MUL [BP-2], [BP-1]
            MOV FX, BP
            SUB FX, 2
            MOV SP, BP
            POP BP
            RET