\\ASM DATA=15 EXTRA=30 STACK=500

MAIN: PUSH 5 
    PUSH 2  
    CALL POT
    ADD SP,2
    MOV [10],AX
    MOV AX,1
    OR  AX,%800
    MOV CX,1
    MOV DX,10  
    SYS 2 
    STOP
    
POT: PUSH BP
    MOV BP, SP
    MOV BX, [BP+2] 
    CMP BX, 0      
    JP SIGUE
    MOV AX,1
    JMP FIN

SIGUE: SUB BX,1
      PUSH [BP+3] 
      PUSH BX     
      CALL POT
      MUL AX,[BP+3] 
      JMP FIN
      
FIN: MOV SP, BP
     POP BP 
     RET
