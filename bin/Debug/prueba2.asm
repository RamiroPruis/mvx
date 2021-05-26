\\ASM
hola EQU "hola"
chau EQU "CHAU"

MAIN:       RND 10
            MOV [10], AC
            MOV AX, %800
            OR AX, %001
            MOV DX, 10
            MOV CX, 1
            SYS 2

            XOR [10],[10]
            SYS 2

            LDH 3
            LDL hola
            MOV FX, AC
            MOV [50], FX
            SLEN [10], [50]
            MOV DX, 10
            MOV CX, 1
            SYS 2

            LDH 3
            LDL chau
            MOV EX, AC
            MOV [51], EX
            SCMP [50], [51]
            MOV BX, 20
            MOV [BX], AC
            MOV DX, BX
            SYS 2

            SMOV [50], [51]
            LDH 3
            LDL hola
            MOV DX, AC
            SYS 4

            STOP