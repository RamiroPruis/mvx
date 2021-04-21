#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decoinst.h"

//Arranca .h
int RAM[4096];
int REG[16];

void traduceOperandos(int, int, int **, int **);
void decInstruccion(int, int *, int *);
void MOV(int *, int *);
void ADD(int *, int *);
void SUB(int *, int *);
void MUL(int *, int *);
void DIV(int *, int *);
void SWAP(int *, int *);
void CMP(int *, int *);
void SHL(int *, int *);
void SHR(int *, int *);
void NOT(int *, int *);
void STOP(int *, int *);
void JNN(int *, int *);
void JNP(int *, int *);
void JNZ(int *, int *);
void JN(int *, int *);
void JP(int *, int *);
void JZ(int *, int *);
void JMP(int *, int *);
void XOR(int *, int *);
void OR(int *, int *);
void AND(int *, int *);
void LDL(int *, int *);
void LDH(int *, int *);
void RND(int *, int *);
void SYS(int *, int *);

void (*vecFunciones[25])(int *, int *);
void cargaFunciones();
//Termina .h

//Arranca main
int main(/*char *argv[]*/)
{

    int instruccion;
    int i = 0;
    int mnemo, cantOperandos;
    int *voA, *voB;

    /*
    if ((arch = fopen(argv[1], "rb")) == NULL)
        return 1;
    while(fread(RAM[i],sizeof(int),1,arch)!=NULL)
      i++;
    REG[0] = i-1; //DS
    */

    REG[5] = 0; //IP
    cargaFunciones();

    while (REG[5] >= 0 && REG[5] < REG[0])
    {
        //Obtener proxima instruccion
        instruccion = RAM[REG[5]];
        REG[5]++;
        decInstruccion(instruccion, &cantOperandos, &mnemo);
        traduceOperandos(instruccion, cantOperandos, &voA, &voB);
        vecFunciones[mnemo](voA, voB); //Ejecuta
    }

    return 0;
}

//Termina main

//Arranca .c

//Termina .c

void traduceOperandos(int instruccion, int cantOperandos, int **voA, int **voB)
{
    int toA = -1, toB = -1;
    int voAux = 0;
    int voBux = 0;

    static int *voAStatic;
    static int *voBStatic;

    //Cargamos los tipos de oprando
    if (cantOperandos == 2)
    {
        toA = (instruccion >> 26) & 0x00000003;
        toB = (instruccion >> 24) & 0x00000003;
        voAux = (instruccion >> 12) & 0x00000FFF;
        voBux = instruccion & 0x00000FFF;
        //       A
        if (toA == 0x00)
        {
            *voA = voAux;
            *voA = &voA;
        }

        else if (toA == 0x01)
            **voA = voAux;

        else
            **voA = RAM[(voAux + REG[0])];

        //      B
        if (toB == 0x00)
        {
            *voBStatic = voBux;
            **voB = &voBStatic;
        }
        else if (toB == 0x01)
            **voB = voBux;
        else
            **voB = RAM[(voBux + REG[0])];
    }
    else if (cantOperandos == 1)
    {
        toA = (instruccion >> 22) & 0x00000003;
        voAux = instruccion & 0x0000FFFF;
        //       A
        if (toA == 0x00)
        {
            *voA = voAux;
            *voA = &voA;
        }
        else if (toA == 0x01)
            **voA = voAux;

        else
            **voA = RAM[(voAux + REG[0])];
    }
}

void decInstruccion(int instruccion, int *cantOperando, int *codigo)
{

    if ((instruccion >> 24) == 0xFF)
    {
        //codigo 0 op
        *cantOperando = 0;
        *codigo = instruccion >> 20;
        *codigo = *codigo - 4056; //Para el vector Funciones
    }
    else if ((instruccion >> 28) == 0xF)
    {
        //codigo 1op
        *cantOperando = 1;
        *codigo = instruccion >> 24;
        *codigo = *codigo - 228;
    }
    else
    {
        //codigo 2op
        *cantOperando = 2;
        *codigo = instruccion >> 28;
    }
}
void cargaFunciones()
{
    vecFunciones[0] = MOV;
    vecFunciones[1] = ADD;
    vecFunciones[2] = SUB;
    vecFunciones[3] = SWAP;
    vecFunciones[4] = MUL;
    vecFunciones[5] = DIV;
    vecFunciones[6] = CMP;
    vecFunciones[7] = SHL;
    vecFunciones[8] = SHR;
    vecFunciones[9] = AND;
    vecFunciones[10] = OR;
    vecFunciones[11] = XOR;
    vecFunciones[12] = SYS;
    vecFunciones[13] = JMP;
    vecFunciones[14] = JZ;
    vecFunciones[15] = JP;
    vecFunciones[16] = JN;
    vecFunciones[17] = JNZ;
    vecFunciones[18] = JNP;
    vecFunciones[19] = JNN;
    vecFunciones[20] = LDL;
    vecFunciones[21] = LDH;
    vecFunciones[22] = RND;
    vecFunciones[23] = NOT;
    vecFunciones[24] = STOP;
}

void cambiaCC(int val)
{
    if (val == 0)
        REG[8] = 1;
    else if (val < 0)
        REG[8] = 0x80000000;
    else
        REG[8] = 0;
}

//OPERACIONES
void MOV(int *valA, int *valB)
{
    *valA = *valB;
}

void ADD(int *valA, int *valB)
{

    *valA = *valA + *valB;
    cambiaCC(*valA);
}

void SUB(int *valA, int *valB)
{

    *valA = *valA - *valB;

    cambiaCC(*valA);
}

void MUL(int *valA, int *valB)
{

    *valA = *valA * (*valB);

    cambiaCC(*valA);
}

void DIV(int *valA, int *valB)
{

    *valA = (int)*valA / (*valB);
    REG[9] = *valA % (*valB);
    cambiaCC(*valA);
}

void SWAP(int *valA, int *valB)
{
    int aux = *valA;

    *valA = *valB;
    *valB = aux;
}

void CMP(int *valA, int *valB)
{
    int aux = *valA - *valB;

    cambiaCC(aux);
}

void SHL(int *valA, int *valB)
{

    *valA = *valA << *valB;

    cambiaCC(*valA);
}

void SHR(int *valA, int *valB)
{

    *valA = *valA >> *valB;

    cambiaCC(*valA);
}

void AND(int *valA, int *valB)
{
    (*valA) = *valA & *valB;
    cambiaCC(*valA);
}

void OR(int *valA, int *valB)
{
    *valA = *valA | *valB;
    cambiaCC(*valA);
}

void XOR(int *valA, int *valB)
{
    *valA = *valA ^ *valB;
    cambiaCC(*valA);
}

void JMP(int *valA, int *valB)
{
    REG[5] = *valA;
}

void JZ(int *valA, int *valB)
{
    if (REG[8] == 1)
        REG[5] = *valA;
}

void JP(int *valA, int *valB)
{
    if (REG[8] > 0)
        REG[5] = *valA;
}

void JN(int *valA, int *valB)
{
    if (REG[8] == 0x80000000)
        REG[5] = *valA;
}

void JNZ(int *valA, int *valB)
{
    if (REG[8] == 0)
        REG[5] = *valA;
}

void JNP(int *valA, int *valB)
{
    if (REG[8] == 0x8000000 || REG[8] == 1)
        REG[5] = *valA;
}

void JNN(int *valA, int *valB)
{
    if (REG[8] == 0 || REG[8] == 1)
    {
        REG[5] = *valA;
    }
}

void NOT(int *valA, int *valB)
{

    *valA = ~(*valA);
    cambiaCC(*valA);
}

void STOP(int *valA, int *valB)
{

    REG[5] = REG[0];
}

void LDL(int *valA, int *valB)
{

    REG[9] = REG[9] & 0xFFFFFF00;
    *valA = *valA & 0x000000FF;
    REG[9] = REG[9] | *valA;
}

void LDH(int *valA, int *valB)
{
    REG[9] = REG[9] & 0x00FFFFFF;
    *valA = (*valA & 0x000000FF) << 24;
    REG[9] = REG[9] | *valA;
}

void RND(int *valA, int *valB)
{
    *valA = rand() % *valB;
}

void SYS(int *valA, int *valB)
{

    char *vec;
    int *vecInt;
    int idInicia, condicion;
    int i = 1;
    int bitPrompt, bitEndLine, bitHexa, bitOctal, bitDecimal, bitDefault;
    //Caso LECTURA
    if (*valA == 1)
    {
        //Ahora reservamos memoria dinamica del vector dependiendo CX==reg[6], acordarse de al final hacer un free
        vec = (char *)malloc(sizeof(char) * REG[6]);

        //if ((REG[4] & 0x0400)!=0) 0000 0100 0000 0000
        if (!((REG[4] >> 11) & 0x00000001)) //Prompt
        {
            idInicia = REG[7];
            while (i <= REG[6])
            {
                printf("[%04d]:", idInicia);
                scanf("%c", vec);
                vec++;
                idInicia++;
                printf("\n");
            }
        }
        else //Sin prompt
        {
            while (i <= REG[6])
            {
                scanf("%c", vec);
                vec++;
                printf("\n");
            }
        }
        //Ahora vemos como tenemos que interpretar su contenido
        //if ((REG[4] & 0x8) != 0)
        if (!((REG[4] >> 8) & 0x00000001))
        {
            idInicia = REG[7];
            //Debemos interpretar (hexa,oct,decimal)
            //Hexa
            if ((REG[4] >> 3) & 0x00000001)
            {
                for (int i = 0; i < REG[6]; i++)
                {
                    RAM[idInicia] = strtol(*vec, NULL, 16); //Cambia de string a la base deseada en int
                    vec++;
                    idInicia++;
                }
            }
            //Octal
            else if ((REG[4] >> 2) & 0x00000001)
            {
                for (int i = 0; i < REG[6]; i++)
                {
                    RAM[idInicia] = strtol(*vec, NULL, 8);
                    vec++;
                    idInicia++;
                }
            }
            //Decimal
            else if (REG[4] & 0x00000001)
            {
                for (int i = 0; i < REG[6]; i++)
                {
                    RAM[idInicia] = strtol(*vec, NULL, 10);
                    vec++;
                    idInicia++;
                }
            }
            else
                printf("Error. No especifica en que sistema numerico interpretar los datos\n");
        }
        else
        {
            //Añadimos a la memoria tal y como esta el vector
            for (int i = 0; i < REG[6]; i++)
            {
                RAM[idInicia] = *vec;
                vec++;
                idInicia++;
            }
        }
        RAM[idInicia] = '%00'; //Caracter final (comun a todos los casos) (menos para los errores, ver que onda)
        free(vec);             //Para todos los casos
    }

    //Caso ESCRITURA
    else if (*valA == 2)
    {
        idInicia = REG[7]; //DX
        bitPrompt = (REG[4] >> 11) & 0x00000001;
        bitEndLine = (REG[4] >> 8) & 0x00000001;
        bitDefault = (REG[4] >> 4) & 0x00000001;
        bitHexa = (REG[4] >> 3) & 0x00000001;
        bitOctal = (REG[4] >> 2) & 0x00000001;
        bitDecimal = REG[4] & 0x00000001;

        if (bitDecimal || bitDefault || bitHexa || bitOctal)
        {
            for (int i = 0; i < REG[6]; i++)
            {
                //Con prompt
                if (bitPrompt)
                {

                    if (bitEndLine)
                    {
                        if (bitHexa)
                            printf("[%04d]: %x\n", idInicia, RAM[idInicia]);
                        else if (bitOctal)
                            printf("[%04d]: %o\n", idInicia, RAM[idInicia]);
                        else if (bitDecimal)
                            printf("[%04d]: %d\n", idInicia, RAM[idInicia]);
                        else
                            printf("[%04d]: %c\n", idInicia, RAM[idInicia]);
                    }
                    else
                    {
                        if (bitHexa)
                            printf("[%04d]: %x", idInicia, RAM[idInicia]);
                        else if (bitOctal)
                            printf("[%04d]: %o", idInicia, RAM[idInicia]);
                        else if (bitDecimal)
                            printf("[%04d]: %d", idInicia, RAM[idInicia]);
                        else
                            printf("[%04d]: %c", idInicia, RAM[idInicia]);
                    }
                }
                //Sin prompt
                else
                {
                    if (bitEndLine)
                    {
                        if (bitHexa)
                            printf("%x\n", idInicia, RAM[idInicia]);
                        else if (bitOctal)
                            printf("%o\n", idInicia, RAM[idInicia]);
                        else if (bitDecimal)
                            printf("%d\n", idInicia, RAM[idInicia]);
                        else
                            printf("c\n", idInicia, RAM[idInicia]);
                    }
                    else
                    {
                        if (bitHexa)
                            printf("%x", idInicia, RAM[idInicia]);
                        else if (bitOctal)
                            printf("%o", idInicia, RAM[idInicia]);
                        else if (bitDecimal)
                            printf("%d", idInicia, RAM[idInicia]);
                        else
                            printf("c", idInicia, RAM[idInicia]);
                    }
                }
                idInicia++;
            }
            if (!bitEndLine)
                printf("\n"); //Para que haga un salto de linea
        }
        else
            printf("Error, no se indica como interpretar el contenido para la escritura\n");
    }
    else
        printf("Error, SYS no interpreta ese argumento\n");
}
