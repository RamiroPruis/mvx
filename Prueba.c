#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Comienza .h
typedef struct
{
    char mnemo[10];
    int hex;
} Tvec;

typedef struct
{
    char cadena[100];
} Tdisasembler;

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
void SLEN(int *, int *);
void SMOV(int *, int *);
void SCMP(int *, int *);
void SYS(int *, int *);
void PUSH(int *, int *);
void POP(int *, int *);
void CALL(int *, int *);
void RET(int *, int *);

void creadicc(Tvec[]);
void creaReg(Tvec[]);
void (*vecFunciones[32])(int *, int *);
void cargaFunciones();
void pasoApaso();
void desarmaPalabra(char[], char[], char[]);
void muestraValor(char[]);
void pasoApaso(char[]);
void dissasembler(int, int *);
void traduceIntruccion(char[], int, Tvec[], Tvec[], int *);
void BuscaRegistro(int, int *, Tvec[]);
void trunca(int *, int);
void proxinstruccion();
void iniciaEjecucion(FILE *, int *);
int getPosicionAbsoluta(int);
int devuelveIndirecto(int);
int getParteAlta(int);
int getParteBaja(int);
void setParteBaja(int *, int);
void setParteAlta(int *, int);

//Termina .h

//Empieza main
int RAM[8192];
int REG[16];
int voAStaticVal, voBStaticVal;
int *voAStatic = &voAStaticVal;
int *voBStatic = &voBStaticVal;
Tdisasembler DISASEMBLER[1000];
int flagB = 0;
int flagC = 0;
int flagD = 0;
Tvec vecReg[16];
Tvec vecMnemo[32];

int main(/*int argc, char *argv[]*/)
{
    FILE *arch;
    //  int instruccion;
    int i = 0, j = 0;
    // int mnemo, cantOperandos;
    //  int voAval, voBval;
    //  int *voA = &voAval, *voB = &voBval;
    //size_t len = strlen(argv[1]);
    //const char *bin = &argv[1][len - 4];

    // if (argc<2)
    //   printf("Error. Faltan argumentos. Recomendacion:\n mvx.exe BinFilename [-b] [-c] [-d] (flags opcionales[]) \n");
    // else
    //   if (strcmp(bin,".bin")!=0) //SI LA EXTENSION NO ES .BIN
    //       printf("Error. El archivo binario no es de tipo .bin \n");
    //   else
    //       if (argc>2){
    //           j=3;
    //           while (j<=argc){
    //               if (strcmp(argv[j-1],"-b")==0)
    //                   flagB=1;
    //               else
    //                   if (strcmp(argv[j-1],"-c")==0)
    //                       flagC=1;

    //                   else
    //                       flagD=1;
    //               j++;
    //           }
    //       }
    flagD = 1;
    flagB = 1;
    if ((arch = fopen("Ejercicios assembler\\Ej4.bin", "rb")) == NULL)
        return 1;

    //Encabezado
    cargaFunciones();
    iniciaEjecucion(arch, &i);
    if (i == 0)
    {
        creadicc(vecMnemo);
        creaReg(vecReg);
        while (fread(&RAM[i], sizeof(int), 1, arch) == 1)
            i++;
        if (flagD)
        {
            printf("Codigo:\n");
            j = 0;
            while (j < i)
            {
                dissasembler(RAM[j], &j);
                j++;
            }
        }
    }
    //Si hubo algun problema con el encabezado no se ejecuta el programa
    else
        return 1;

    int ds = getParteBaja(REG[0]);
    int cs = getParteBaja(REG[3]);
    REG[5] = getParteBaja(REG[3]); //IP INICALIZADO EN CS

    if (flagD)
    {
        //mostramos por primera vez
        for (int i = 0; i < ds; i++)
            if (DISASEMBLER[i].cadena[0])
                printf("%s\n", DISASEMBLER[i].cadena);
        printf("\n");
    }

    while (REG[5] >= cs && REG[5] < ds)
    {
        proxinstruccion();
    }

    return 0;
}

//Termina main

//Arranca .c
void creadicc(Tvec vec[])
{
    strcpy(vec[0].mnemo, "MOV");
    vec[0].hex = 0x00;
    strcpy(vec[1].mnemo, "ADD");
    vec[1].hex = 0x01;
    strcpy(vec[2].mnemo, "SUB");
    vec[2].hex = 0x02;
    strcpy(vec[3].mnemo, "SWAP");
    vec[3].hex = 0x03;
    strcpy(vec[4].mnemo, "MUL");
    vec[4].hex = 0x04;
    strcpy(vec[5].mnemo, "DIV");
    vec[5].hex = 0x05;
    strcpy(vec[6].mnemo, "CMP");
    vec[6].hex = 0x06;
    strcpy(vec[7].mnemo, "SHL");
    vec[7].hex = 0x07;
    strcpy(vec[8].mnemo, "SHR");
    vec[8].hex = 0x08;
    strcpy(vec[9].mnemo, "AND");
    vec[9].hex = 0x09;
    strcpy(vec[10].mnemo, "OR");
    vec[10].hex = 0x0A;
    strcpy(vec[11].mnemo, "XOR");
    vec[11].hex = 0x0B;
    strcpy(vec[12].mnemo, "SYS");
    vec[12].hex = 0xF0;
    strcpy(vec[13].mnemo, "JMP");
    vec[13].hex = 0xF1;
    strcpy(vec[14].mnemo, "JZ");
    vec[14].hex = 0xF2;
    strcpy(vec[15].mnemo, "JP");
    vec[15].hex = 0xF3;
    strcpy(vec[16].mnemo, "JN");
    vec[16].hex = 0xF4;
    strcpy(vec[17].mnemo, "JNZ");
    vec[17].hex = 0xF5;
    strcpy(vec[18].mnemo, "JNP");
    vec[18].hex = 0xF6;
    strcpy(vec[19].mnemo, "JNN");
    vec[19].hex = 0xF7;
    strcpy(vec[20].mnemo, "LDL");
    vec[20].hex = 0xF8;
    strcpy(vec[21].mnemo, "LDH");
    vec[21].hex = 0xF9;
    strcpy(vec[22].mnemo, "RND");
    vec[22].hex = 0xFA;
    strcpy(vec[23].mnemo, "NOT");
    vec[23].hex = 0xFB;
    strcpy(vec[24].mnemo, "STOP");
    vec[24].hex = 0xFF1;
    strcpy(vec[25].mnemo, "SLEN");
    vec[25].hex = 0x0C;
    strcpy(vec[26].mnemo, "SMOV");
    vec[26].hex = 0x0D;
    strcpy(vec[27].mnemo, "SCMP");
    vec[27].hex = 0x0E;
    strcpy(vec[28].mnemo, "PUSH");
    vec[28].hex = 0xFC;
    strcpy(vec[29].mnemo, "POP");
    vec[29].hex = 0xFD;
    strcpy(vec[30].mnemo, "CALL");
    vec[30].hex = 0xFE;
    strcpy(vec[31].mnemo, "RET");
    vec[31].hex = 0xFF0;
}

void creaReg(Tvec registros[])
{
    //Inicializamos en null
    for (int i = 0; i < 16; i++)
        strcpy(registros[i].mnemo, "\0");

    strcpy(registros[0].mnemo, "DS");
    registros[0].hex = 0;
    strcpy(registros[1].mnemo, "SS");
    registros[1].hex = 1;
    strcpy(registros[2].mnemo, "ES");
    registros[2].hex = 2;
    strcpy(registros[3].mnemo, "CS");
    registros[3].hex = 3;
    strcpy(registros[4].mnemo, "HP");
    registros[4].hex = 4;
    strcpy(registros[5].mnemo, "IP");
    registros[5].hex = 5;
    strcpy(registros[6].mnemo, "SP");
    registros[6].hex = 6;
    strcpy(registros[7].mnemo, "BP");
    registros[7].hex = 7;
    strcpy(registros[8].mnemo, "CC");
    registros[8].hex = 8;
    strcpy(registros[9].mnemo, "AC");
    registros[9].hex = 9;
    strcpy(registros[10].mnemo, "AX");
    registros[10].hex = 10;
    strcpy(registros[11].mnemo, "BX");
    registros[11].hex = 11;
    strcpy(registros[12].mnemo, "CX");
    registros[12].hex = 12;
    strcpy(registros[13].mnemo, "DX");
    registros[13].hex = 13;
    strcpy(registros[14].mnemo, "EX");
    registros[14].hex = 14;
    strcpy(registros[15].mnemo, "FX");
    registros[15].hex = 15;
}
void traduceOperandos(int instruccion, int cantOperandos, int **voA, int **voB)
{
    int toA = -1, toB = -1;
    int voAux = 0;
    int voBux = 0;

    //Cargamos los tipos de oprando
    if (cantOperandos == 2)
    {
        toA = (instruccion >> 26) & 0x00000003;
        toB = (instruccion >> 24) & 0x00000003;
        voAux = (instruccion >> 12) & 0x00000FFF;
        trunca(&voAux, 12);
        voBux = instruccion & 0x00000FFF;
        trunca(&voBux, 12);
        //       A
        if (toA == 0x00) //INMEDIATO
        {
            *voAStatic = voAux;
            *voA = voAStatic;
        }
        else if (toA == 0x01) //DE REGISTRO
            *voA = &REG[voAux];
        else if (toA == 0x02) //DIRECTO
            *voA = &RAM[getPosicionAbsoluta(voAux)];
        else
        { //OP INDIRECTO
            *voA = &RAM[devuelveIndirecto(voAux)];
        }

        //      B
        if (toB == 0x00)
        {
            *voBStatic = voBux;
            *voB = voBStatic;
        }
        else if (toB == 0x01)
            *voB = &REG[voBux];
        else if (toB == 0x02)
            *voB = &RAM[getPosicionAbsoluta(voBux)];
        else //INDIRECTO
            *voB = &RAM[devuelveIndirecto(voBux)];
    }
    else if (cantOperandos == 1)
    {
        toA = (instruccion >> 22) & 0x00000003;
        voAux = instruccion & 0x0000FFFF;
        trunca(&voAux, 16);
        if (toA == 0x00)
        {
            *voAStatic = voAux;
            *voA = voAStatic;
        }
        else if (toA == 0x01)
            *voA = &REG[voAux];
        else if (toA == 0x02)
            *voA = &RAM[getPosicionAbsoluta(voAux)];
        else
            *voA = &RAM[devuelveIndirecto(voAux)];
    }
}

void decInstruccion(int instruccion, int *cantOperando, int *codigo)
{
    if (((instruccion >> 24) & 0xFF) == 0xFF)
    {
        //codigo 0 op
        *cantOperando = 0;
        *codigo = (instruccion >> 20) & 0xFFF;
        *codigo = *codigo - 4050; //Para el vector Funciones
    }
    else if (((instruccion >> 28) & 0xF) == 0xF)
    {
        //codigo 1op
        *cantOperando = 1;
        *codigo = (instruccion >> 24) & 0xFF;
        *codigo = *codigo - 225;
    }
    else
    {
        //codigo 2op
        *cantOperando = 2;
        *codigo = (instruccion >> 28) & 0xF;
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
    vecFunciones[12] = SLEN;
    vecFunciones[13] = SMOV;
    vecFunciones[14] = SCMP;
    vecFunciones[15] = SYS;
    vecFunciones[16] = JMP;
    vecFunciones[17] = JZ;
    vecFunciones[18] = JP;
    vecFunciones[19] = JN;
    vecFunciones[20] = JNZ;
    vecFunciones[21] = JNP;
    vecFunciones[22] = JNN;
    vecFunciones[23] = LDL;
    vecFunciones[24] = LDH;
    vecFunciones[25] = RND;
    vecFunciones[26] = NOT;
    vecFunciones[27] = PUSH;
    vecFunciones[28] = POP;
    vecFunciones[29] = CALL;
    vecFunciones[30] = RET;
    vecFunciones[31] = STOP;
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
    if (REG[8] == 0)
        REG[5] = *valA;
}

void JN(int *valA, int *valB)
{
    if (REG[8] == 0x80000000)
        REG[5] = *valA;
}

void JNZ(int *valA, int *valB)
{
    if (REG[8] != 1)
        REG[5] = *valA;
}

void JNP(int *valA, int *valB)
{
    if (REG[8] == 0x80000000 || REG[8] == 1)
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

    *valA = ~(*valA) + 1;
    cambiaCC(*valA);
}

void STOP(int *valA, int *valB)
{

    REG[5] = getParteBaja(REG[0]);
}

void LDL(int *valA, int *valB)
{
    int aux = *valA & 0xFFFF;
    REG[9] = REG[9] & 0xFFFF0000;
    REG[9] |= aux;
}

void LDH(int *valA, int *valB)
{
    int aux = (*valA & 0xFFFF) << 16;
    REG[9] = REG[9] & 0x0000FFFF;
    REG[9] |= aux;
}

void RND(int *valA, int *valB)
{
    REG[9] = rand() % *valA;
}

void PUSH(int *valA, int *valB)
{
    //Si no estamos en la ultima posicion del SS
    if (getParteBaja(REG[6]) > 0)
    {
        setParteBaja(&REG[6], getParteBaja(REG[6]) - 1); //Decrementamos el SP (solo la direccion, no tocamos el segmento)
        RAM[getPosicionAbsoluta(REG[6])] = *valA;
    }
    //Caso de OverFlow
    else
    {
        printf("OverFlow en la pila\n");
        exit(1);
    }
}

void POP(int *valA, int *valB)
{
    //Si la pila no esta vacia podemos sacar
    if (getParteBaja(REG[6]) < getParteAlta(REG[1]))
    {
        *valA = RAM[getPosicionAbsoluta(REG[6])];
        setParteBaja(&REG[6], getParteBaja(REG[6]) + 1); //Aumentamos el SP (solo la direccion, no tocamos el segmento)
    }
    else
    {
        printf("UnderFlow en la pila\n");
        exit(1);
    }
}

void CALL(int *valA, int *valB)
{
    PUSH(&REG[5], &REG[5]); //Pusheamos la direccion de memoria de la siguiente instruccion que se iba a ejecutar, ip siempre apunta a la siguiente instruccion cuando se esta ejecutando una
    REG[5] = *valA;
}

void RET(int *valA, int *valB)
{
    POP(&REG[5], &REG[5]);
}

void SLEN(int *valA, int *valB)
{
    int largo = 0, pos;
    pos = *valB; //posicion del primer char
    while (RAM[pos] != '\0')
    {
        largo++;
        pos++;
    }
    largo++; //incluir \0
    *valA = largo;
}

void SMOV(int *valA, int *valB)
{
    int posA = *valA, posB = *valB;

    while (RAM[posB] != '\0')
    {
        RAM[posA] = RAM[posB];
        posA++;
        posB++;
    }
    RAM[posA] = '\0';
}

void SCMP(int *valA, int *valB)
{
    int posA = *valA, posB = *valB;

    do
    {
        REG[8] = RAM[posA] - RAM[posB];
        posA++;
        posB++;
    } while (RAM[posA] != '\0' && RAM[posB] != '\0' && REG[8] != 0);
}

void SYS(int *valA, int *valB)
{
    char rta[20] = {"\0"};
    char prompt[10] = "";
    char cad[500] = {"\0"};
    char cad2[500] = {""};
    char caracter;
    char entrada[100];
    int i, condChar = 0;

    if ((REG[10] & 0x800) == 0)
        strcpy(prompt, "[%04d]: ");
    else
        strcpy(prompt, "");

    //Caso LECTURA
    if (*valA == 1)
    {
        if ((REG[10] & 0x100) != 0)
        {
            strcat(cad, "%c");
            condChar = 1;
        }
        else
        {
            if ((REG[10] & 0x8) != 0)
                strcat(cad, " %x");
            if ((REG[10] & 0x4) != 0)
                strcat(cad, " %o");
            if ((REG[10] & 0x1) != 0)
                strcat(cad, " %d");
        }

        if (condChar == 0)
            for (i = 0; i < REG[12]; i++)
            {
                printf(prompt, getPosicionAbsoluta(REG[13] + i));
                scanf(cad, &RAM[getPosicionAbsoluta(REG[13] + i)]);
            }
        else
        {
            printf(prompt, getPosicionAbsoluta(REG[13]));
            scanf("%s", entrada);
            for (i = 0; entrada[i]; i++)
                RAM[getPosicionAbsoluta(REG[13] + i)] = entrada[i]; //Aunque no sea eficiente llamar a getPosicion para solo acceder a la siguiente, hay que hacerlo por un posible segmentation fault
        }
    }
    //Caso ESCRITURA
    else if (*valA == 2)
    {
        if ((REG[10] & 0x10) != 0)
            strcat(cad2, " %c");
        if ((REG[10] & 0x8) != 0)
            strcat(cad, "  % %%08X");
        if ((REG[10] & 0x4) != 0)
            strcat(cad, " @%08o");
        if ((REG[10] & 0x1) != 0)
            strcat(cad, " %d");
        if ((REG[10] & 0x100) == 0)
            strcat(cad, " \n");

        int posicion = getPosicionAbsoluta(REG[13]); //Aca si podemos usar un auxiliar
        for (i = 0; i < REG[12] && RAM[posicion + i] != '\0'; i++)
        {

            printf(prompt, posicion + i);
            if (RAM[posicion + i] >= 32)
                caracter = RAM[posicion + i];
            else
                caracter = '.';
            printf(cad2, caracter);
            printf(cad, RAM[posicion + i], RAM[posicion + i], RAM[posicion + i]);
        }
    }
    else if (*valA == 3)
    {
        //STRING READ
        strcat(cad, " %s");
        scanf(cad, entrada);
        i = 0;
        while (entrada[i] != '\0' && i < REG[12])
        {
            RAM[getPosicionAbsoluta(REG[13] + i)] = entrada[i];
            i++;
        }
        RAM[getPosicionAbsoluta(REG[13] + i)] = '\0';
    }
    else if (*valA == 4)
    {
        //STRING WRITE
        char salida[100];
        i = 0;
        int pos = getPosicionAbsoluta(REG[13]);
        strcat(cad, " %s");
        if ((REG[10] & 0x80) == 0)
            strcat(cad, "\n");
        while (RAM[pos + i] != '\0')
        {
            salida[i] = RAM[pos + i];
            i++;
        }
        salida[i] = '\0';
        printf(cad, salida);
    }
    else if (*valA == 5) // NEW(CX) -> DX
    {
        int errorOverflow = 0, encontroEspacio = 0, posRAM, i, j, anterior;

        //caso inicial
        if (REG[4] == 0xFFFFFFFF)
        {
            setParteAlta(&RAM[getParteBaja(REG[2])], getParteAlta(REG[2]) - 1);
            setParteBaja(&RAM[getParteBaja(REG[2])], 0);
            setParteAlta(&REG[4], 0);
            setParteBaja(&REG[4], 0xFFFF);
        }

        if (getParteAlta(REG[4]) != -1)
        {
            posRAM = getParteAlta(REG[4]) + getParteBaja(REG[2]); //puntero a Lista de Libres
            while (encontroEspacio == 0 && errorOverflow == 0)
            {
                if (getParteAlta(RAM[posRAM]) > REG[12])
                {
                    // Encontro espacio y divide el bloque
                    encontroEspacio = 1;
                    j = getParteBaja(REG[4]) + getParteBaja(REG[2]); //puntero a Lista de Ocupados
                    anterior = -1;
                    if (j != (0xFFFF + getParteBaja(REG[2])))
                        do
                        {
                            if ((j == getParteBaja(RAM[j]) + getParteBaja(REG[2])) || (j > getParteBaja(RAM[j]) + getParteBaja(REG[2]) && (posRAM < getParteBaja(RAM[j]) + getParteBaja(REG[2]) || posRAM > j)) || (posRAM < getParteBaja(RAM[j]) + getParteBaja(REG[2]) && posRAM > j))
                                anterior = j;
                            else
                                j = getParteBaja(RAM[j]) + getParteBaja(REG[2]);
                        } while (anterior == -1);
                    else
                        anterior = posRAM;

                    // HEADER nuevo bloque vacio
                    if (posRAM == getParteBaja(RAM[posRAM]) + getParteBaja(REG[2]))
                        setParteBaja(&RAM[posRAM + REG[12] + 1], posRAM + REG[12] + 1 - getParteBaja(REG[2]));
                    else
                        setParteBaja(&RAM[posRAM + REG[12] + 1], getParteBaja(RAM[posRAM]));
                    setParteAlta(&RAM[posRAM + REG[12] + 1], getParteAlta(RAM[posRAM]) - REG[12] - 1);

                    // modifica HEADER que apuntaba al bloque vacio
                    i = posRAM;
                    if (getParteBaja(RAM[i]) + getParteBaja(REG[2]) != i)
                    {
                        while ((getParteBaja(RAM[i]) + getParteBaja(REG[2])) != posRAM)
                            i = getParteBaja(RAM[i]) + getParteBaja(REG[2]);
                        setParteBaja(&RAM[i], posRAM - getParteBaja(REG[2]) + REG[12] + 1); //apunta al nuevo nodo vacio
                    }

                    // HEADER nuevo bloque ocupado
                    if (posRAM == anterior)
                        setParteBaja(&RAM[posRAM], anterior - getParteBaja(REG[2]));
                    else
                        setParteBaja(&RAM[posRAM], getParteBaja(RAM[anterior]));
                    setParteAlta(&RAM[posRAM], REG[12]);

                    // modifica HEADER del bloque ocupado anterior
                    setParteBaja(&RAM[anterior], posRAM - getParteBaja(REG[2]));

                    // actualiza HPH
                    if (posRAM == getParteAlta(REG[4]) + getParteBaja(REG[2]))
                        setParteAlta(&REG[4], getParteAlta(REG[4]) + REG[12] + 1);
                    setParteBaja(&REG[4], posRAM - getParteBaja(REG[2])); // actualiza HPL
                    REG[13] = posRAM + 1 - getParteBaja(REG[2]);          // setea DX
                }
                else if (getParteAlta(RAM[posRAM]) == REG[12])
                {
                    // Encontro espacio justo
                    encontroEspacio = 1;

                    if (getParteBaja(REG[4]) != 0xFFFF)
                    {
                        j = getParteBaja(REG[4]) + getParteBaja(REG[2]);
                        anterior = -1;
                        do
                        {
                            if ((j == getParteBaja(RAM[j]) + getParteBaja(REG[2])) || (j > getParteBaja(RAM[j]) + getParteBaja(REG[2]) && (posRAM < getParteBaja(RAM[j]) + getParteBaja(REG[2]) || posRAM > j)) || (posRAM < getParteBaja(RAM[j]) + getParteBaja(REG[2]) && posRAM > j))
                                anterior = j;
                            else
                                j = getParteBaja(RAM[j]) + getParteBaja(REG[2]);
                        } while (anterior == -1);
                    }
                    else
                    {
                        anterior = posRAM;
                    }

                    if (getParteBaja(RAM[posRAM]) == posRAM - getParteBaja(REG[2]))
                    {
                        // Unico espacio libre
                        setParteAlta(&REG[4], 0xFFFF);
                    }
                    else
                    {
                        // HEADER del anterior bloque vacio apunta al siguiente
                        i = posRAM;
                        if (getParteBaja(RAM[i]) + getParteBaja(REG[2]) != i)
                        {
                            while ((getParteBaja(RAM[i]) + getParteBaja(REG[2])) != posRAM)
                                i = getParteBaja(RAM[i]) + getParteBaja(REG[2]);
                            setParteBaja(&RAM[i], getParteBaja(RAM[posRAM]));
                        }
                    }

                    // HEADER nuevo bloque ocupado
                    if (posRAM == anterior)
                        setParteBaja(&RAM[posRAM], anterior - getParteBaja(REG[2]));
                    else
                        setParteBaja(&RAM[posRAM], getParteBaja(RAM[anterior]));

                    // actualiza HPH
                    if (getParteBaja(RAM[posRAM]) != posRAM - getParteBaja(REG[2]))
                        if (posRAM == getParteAlta(REG[4]) + getParteBaja(REG[2]))
                            setParteAlta(&REG[4], getParteBaja(RAM[i]));

                    // HEADER anterior apunta a nuevo bloque ocupado
                    setParteBaja(&RAM[anterior], posRAM - getParteBaja(RAM[2]));

                    setParteBaja(&REG[4], posRAM - getParteBaja(REG[2])); // actualiza HPL
                    REG[13] = posRAM + 1 - getParteBaja(REG[2]);          // setea DX
                }
                else
                {
                    // No encontro espacio
                    if (posRAM == getParteAlta(REG[4]))
                    {
                        errorOverflow = 1; // No hay espacio disponible ERROR
                        printf("ERROR:\tMemoria Insuficiente\n");
                        exit(1);
                    }
                    posRAM = getParteBaja(RAM[posRAM]) + getParteBaja(REG[2]);
                }
            }
            setParteAlta(&REG[13], 2);
        }
        else
        {
            printf("ERROR:\tMemoria Insuficiente\n");
            exit(1);
        }
    }
    else if (*valA == 6) // FREE
    {
        int posRAM;
        int encontroOcupado = 0;
        int antLibre, i, j;
        if (getParteBaja(REG[4]) == 0xFFFF)
            return; // ERROR no hay espacio para liberar
        else
        {
            posRAM = getParteBaja(REG[4]) + getParteBaja(REG[2]); //puntero a Lista de Ocupados
            while (!encontroOcupado)
            {
                if ((posRAM < getPosicionAbsoluta(REG[13])) && (getParteAlta(RAM[posRAM]) + posRAM >= getPosicionAbsoluta(REG[13])))
                {
                    // Encuentra bloque en posRAM
                    encontroOcupado = 1;

                    j = getParteAlta(REG[4]) + getParteBaja(REG[2]); //puntero a Lista de Libres
                    antLibre = -1;
                    if (j != 0xFFFF + getParteBaja(REG[2]))
                        do
                        {
                            if ((j == (getParteBaja(RAM[j]) + getParteBaja(REG[2]))) || (j < posRAM && (getParteBaja(RAM[j]) + getParteBaja(REG[2])) > posRAM) || (j > posRAM && j > (getParteBaja(RAM[j]) + getParteBaja(REG[2])) && (getParteBaja(RAM[j]) + getParteBaja(REG[2])) > posRAM))
                                antLibre = j;
                            else
                                j = getParteBaja(RAM[j]) + getParteBaja(REG[2]);
                        } while (antLibre == -1);
                    else
                        antLibre = posRAM;

                    // actualiza HPL
                    if (getParteBaja(REG[4]) + getParteBaja(REG[2]) == posRAM)
                    {
                        if (getParteBaja(RAM[posRAM]) == posRAM - getParteBaja(REG[2]))
                            setParteBaja(&REG[4], -1);
                        else
                            setParteBaja(&REG[4], getParteBaja(RAM[posRAM]));
                    }

                    // actualiza HPH
                    if (getParteAlta(REG[4]) + getParteBaja(REG[2]) > posRAM)
                        setParteAlta(&REG[4], posRAM - getParteBaja(REG[2]));

                    // cambio HEADER que apuntaba al bloque a liberar
                    i = posRAM;
                    while ((getParteBaja(RAM[i]) + getParteBaja(REG[2])) != posRAM)
                        i = getParteBaja(RAM[i]) + getParteBaja(REG[2]);
                    setParteBaja(&RAM[i], getParteBaja(RAM[posRAM]));

                    setParteBaja(&RAM[posRAM], getParteBaja(RAM[antLibre]));     // cambio HEADER del bloque a liberar
                    setParteBaja(&RAM[antLibre], posRAM - getParteBaja(REG[2])); // cambio HEADER del bloque libre anterior

                    // compuebo si quedan dos bloques libres contiguos
                    if (getParteAlta(RAM[antLibre]) + getParteBaja(REG[2]) + 1 == posRAM)
                    {
                        // Anterior bloque vacio
                        setParteAlta(&RAM[antLibre], getParteAlta(RAM[antLibre]) + getParteAlta(RAM[posRAM]) + 1);
                        setParteBaja(&RAM[antLibre], getParteBaja(RAM[posRAM]));
                        posRAM = antLibre;
                    }
                    if (getParteAlta(RAM[posRAM]) + posRAM + 1 == getParteBaja(RAM[posRAM]) + getParteBaja(REG[2]))
                    {
                        // Siguiente bloque vacio
                        setParteAlta(&RAM[posRAM], getParteAlta(RAM[posRAM]) + getParteAlta(RAM[getParteBaja(RAM[posRAM]) + getParteBaja(REG[2])]) + 1);
                        setParteBaja(&RAM[posRAM], getParteBaja(RAM[getParteBaja(RAM[posRAM]) + getParteBaja(REG[2])]));
                    }
                }
                else
                    posRAM = getParteBaja(RAM[posRAM]) + getParteBaja(REG[2]);
            }
        }
    }
    else if (*valA == 7)
    {
        system("clear");
    }
    else if (*valA == 15)
    { //F

        //Si el flag c esta prendido
        if (flagC)
            system("clear");
        if (flagD)
        {

            //Mostramos 10 lineas a partir del IP 5 antes y 4 despues (y con el 10)
            if (REG[5] - 5 >= 0)
                for (int i = REG[5] - 5; i < REG[5] + 5; i++)
                    if (i == REG[5])
                    {
                        printf(">");
                        printf("%s\n", DISASEMBLER[i].cadena);
                    }
                    else
                        printf(" %s\n", DISASEMBLER[i].cadena);

            else
                for (int i = 0; i < 10; i++)
                    if (i == REG[5])
                    {
                        printf(">");
                        printf("%s\n", DISASEMBLER[i].cadena);
                    }
                    else
                        printf(" %s\n", DISASEMBLER[i].cadena);

            //proxinstruccion();
            sprintf(cad, "Registros:\n");
            int j = 0;
            for (i = 1; i <= 4; i++)
            {
                while (j < i * 4)
                {
                    fflush(stdin);
                    sprintf(cad2, "%s = %15d (%d|%d)|", vecReg[j].mnemo, REG[j], getParteAlta(REG[j]), getParteBaja(REG[j]));
                    j++;
                    strcat(cad, cad2);
                }
                strcat(cad, "\n");
            }
            printf("%s", cad);
        }
        if (flagB)
        {
            printf("[%04d] cmd: ", REG[5]);
            fflush(stdin);
            fgets(rta, 20, stdin);
            rta[strcspn(rta, "\n")] = 0; //Esto creo que le corta el \n
            printf("\n");
            //Ejecucion paso a paso
            if (strcmp(rta, "p") == 0)
                pasoApaso(rta);
            else
                muestraValor(rta);
        }
    }
    else
        printf("Error, SYS no interpreta ese argumento\n");
}

void pasoApaso(char rta[])
{
    int voAval, voBval;
    int *voA = &voAval, *voB = &voBval;
    int instruccion;
    int mnemo, cantOperandos;

    //La primer operacion que se ejecuta es justamente la que le sigue al breakpoint pues en el main ya se hizo el REG[5]++
    while (REG[5] >= 0 && REG[5] < REG[0])
    {
        //Obtener proxima instruccion
        // instruccion = RAM[REG[5]];
        // REG[5]++;
        // decInstruccion(instruccion, &cantOperandos, &mnemo);
        // traduceOperandos(instruccion, cantOperandos, &voA, &voB);
        // printf("[%04d]: %02X %02X %02X %02X\n", REG[5], (instruccion >> 24) & 0xFF, (instruccion >> 16) & 0xFF, (instruccion >> 8) & 0xFF, (instruccion >> 0) & 0xFF);
        // vecFunciones[mnemo](voA, voB); //Ejecuta
        proxinstruccion();
        printf("[%04d] cmd: ", REG[5]);
        fflush(stdin);
        fgets(rta, 20, stdin);
        rta[strcspn(rta, "\n")] = 0;
        printf("\n");
        if (strcmp(rta, "") != 0)
            muestraValor(rta);
    }
}

void muestraValor(char rta[])
{
    char cad1[20] = {"\0"}, cad2[20] = {"\0"};
    int rtaInt1, rtaInt2;

    while (strcmp(rta, "") != 0)
    {
        desarmaPalabra(rta, cad1, cad2);
        //Caso 1 solo valor
        if (strcmp(cad2, "\0") == 0)
        {
            rtaInt1 = strtol(cad1, NULL, 10); //Lo transformamos en int
            printf("[%04d] cmd: %04X %04X %d\n", rtaInt1, (RAM[rtaInt1] >> 16) & 0x0000FFFF, RAM[rtaInt1] & 0x0000FFFF, RAM[rtaInt1]);
        }
        //Caso rango de valores
        else
        {
            rtaInt1 = strtol(cad1, NULL, 10);
            rtaInt2 = strtol(cad2, NULL, 10);
            for (rtaInt1; rtaInt1 <= rtaInt2; rtaInt1++)
                printf("[%04d] cmd: %04X %04X %d\n", rtaInt1, (RAM[rtaInt1] >> 16) & 0x000000FF, RAM[rtaInt1] & 0x000000FF, RAM[rtaInt1]);
        }
        printf("[%04d] cmd: ", REG[5]);
        fflush(stdin);
        fgets(rta, 20, stdin);
        rta[strcspn(rta, "\n")] = 0;
        printf("\n");
        cad1[0] = cad2[0] = '\0';
    }
}

void desarmaPalabra(char rta[], char cad1[], char cad2[])
{
    int i = 0;
    int j = 0, k = 0;
    while (rta[i] != '\0' && rta[i] != ' ')
    {
        cad1[j] = rta[i];
        i++;
        j++;
    }
    i++;
    while (rta[i] != '\0' && rta[i] != ' ')
    {
        cad2[k] = rta[i];
        i++;
        k++;
    }
}

void dissasembler(int instruccion, int *i)
{
    char cad1[100] = {'\0'};
    char cad2[100] = {'\0'};

    sprintf(cad1, "[%04d]: %02X %02X %02X %02X   %d: ", *i, (instruccion >> 24) & 0xFF, (instruccion >> 16) & 0xFF, (instruccion >> 8) & 0xFF, (instruccion >> 0) & 0xFF, *i + 1);
    traduceIntruccion(cad2, instruccion, vecMnemo, vecReg, i); //Retorna en cad2 el "MOV ...."
    strcat(cad1, cad2);                                        //Queda en cad 1 toda la expresion a guardar
    strcpy(DISASEMBLER[*i].cadena, cad1);
}

void traduceIntruccion(char cad[], int inst, Tvec cod[], Tvec reg[], int *val)
{
    int i, j;
    char op1[5] = "\0";
    int aver1 = (inst >> 28) & 0xF;
    int aver2 = cod[11].hex;
    int truncado;
    char str[11];
    //caso string
    if ((inst & 0xFFFFFF00) == 0)
    {
        j = 0;
        while (RAM[*val] != 0)
            str[j++] = RAM[(*val)++];
        str[j] = '\0';
        strcat(cad, "\"");
        strcat(cad, str);
        strcat(cad, "\"");
        return;
    }
    if (((inst >> 24) & 0xFF) == 0x000000FF)
    { // Sin operandos
        i = 24;
        while (cod[i].hex != ((inst >> 20) & 0xFFF))
            i++;
        strcpy(cad, cod[i].mnemo);
    }
    else
    {
        if (((inst >> 28) & 0xF) == 0xF)
        { // 1 operando
            i = 12;
            j = 0;
            while (cod[i].hex != ((inst >> 24) & 0xFF))
                i++;
            strcpy(cad, cod[i].mnemo);
            if (((inst >> 22) & 0x003) == 0x000)
            { // operando inmediato
                truncado = inst & 0x0000FFFF;
                trunca(&truncado, 16);
                sprintf(op1, " %d", truncado);
            }
            else if (((inst >> 22) & 0x003) == 0x001)
            { // operando de registro
                BuscaRegistro(inst & 0x0000FFFF, &j, reg);
                sprintf(op1, " %s", reg[j].mnemo);
            }
            else if (((inst >> 22) & 0x003) == 0x002)
                // operando directo
                sprintf(op1, " [%d]", (inst & 0x0000FFFF));
            else
            { //INDIRECTO
                truncado = (inst & 0x00000FF0) >> 4;
                trunca(&truncado, 8);
                BuscaRegistro(inst & 0x0000000F, &j, reg);
                if (truncado > 0)
                    sprintf(op1, " [%s+%d]", reg[j].mnemo, truncado);
                else
                    sprintf(op1, " [%s-%d]", reg[j].mnemo, ~truncado + 1);
            }
            strcat(cad, op1);
        }
        else
        { // 2 operandos
            i = 0;
            while ((cod[i].hex) != ((inst >> 28) & 0xF))
                i++;
            strcpy(cad, cod[i].mnemo);
            truncado = (inst >> 12) & 0x00FFF;
            trunca(&truncado, 12);
            if (((inst >> 26) & 0x03) == 0x00)
                // operando 1 inmediato
                sprintf(op1, " %d,", truncado);
            else if (((inst >> 26) & 0x03) == 0x01)
            { // operando 1 de registro
                j = 0;
                BuscaRegistro((inst >> 12) & 0x00FFF, &j, reg);
                sprintf(op1, " %s,", reg[j].mnemo);
            }
            else if (((inst >> 26) & 0x03) == 0x02)
                // operando 1 directo
                sprintf(op1, " [%d],", ((inst >> 12) & 0xFFF));
            else
            { //OPERANDO 1 INDIRECTO
                truncado = (inst & 0x00FF0000) >> 16;
                trunca(&truncado, 8);
                BuscaRegistro((inst & 0x0000F000) >> 12, &j, reg);
                if (truncado > 0)
                    sprintf(op1, "[%s+%d]", reg[j].mnemo, truncado);
                else
                    sprintf(op1, "[%s-%d]", reg[j].mnemo, ~truncado + 1);
            }
            strcat(cad, op1);
            truncado = inst & 0x00000FFF;
            trunca(&truncado, 12);
            j = 0;
            if (((inst >> 24) & 0x03) == 0x00)
                // operando 2 inmediato
                sprintf(op1, "%d", truncado);
            else if (((inst >> 24) & 0x03) == 0x01)
            { // operando 2 de registro
                BuscaRegistro((inst & 0x0000000F), &j, reg);
                sprintf(op1, "%s", reg[j].mnemo);
            }
            else if (((inst >> 24) & 0x03) == 0x02)
                // operando 2 directo
                sprintf(op1, "[%d]", (inst & 0x00000FFF));
            else
            { //OPERANDO 2 INDIRECTO
                truncado = (inst & 0x00000FF0) >> 4;
                trunca(&truncado, 8);
                BuscaRegistro((inst & 0x0000000F), &j, reg);
                if (truncado > 0)
                    sprintf(op1, "[%s+%d]", reg[j].mnemo, truncado);
                else
                    sprintf(op1, "[%s-%d]", reg[j].mnemo, ~truncado + 1);
            }
            strcat(cad, op1);
        }
    }
}

void BuscaRegistro(int numreg, int *j, Tvec reg[])
{
    *j = 0;
    while (*j < 15 && reg[*j].hex != numreg)
        (*j)++;
    if (reg[*j].hex != numreg)
        (*j) = -1;
}

void trunca(int *ValorOperando, int bitsmax)
{
    switch (bitsmax)
    {
    case 8:
        if (*ValorOperando > 127 || *ValorOperando < -128)
        {
            if ((*ValorOperando) >> 7)
                *ValorOperando |= 0xFFFFFF00;
            else
                *ValorOperando &= 0x000000FF;
        }
        break;
    case 12:
        if ((*ValorOperando > 2047 || *ValorOperando < -2048))
        {
            if ((*ValorOperando & 0xFFF) >> 11) //Bit mas significativo de los 12bits es un 1 --> numero negativo
                *ValorOperando |= 0xFFFFF000;
            else
                *ValorOperando = (*ValorOperando & 0x00000FFF);
        }
        break;
    case 16:
        if ((*ValorOperando >= 32767 || *ValorOperando <= -32768) && bitsmax == 16)
        {
            if ((*ValorOperando & 0xFFFF) >> 15)
                *ValorOperando |= 0xFFFF0000;
            else
                *ValorOperando &= 0x0000FFFF;
        }
        break;
    }
}

void proxinstruccion()
{
    int voAval, voBval;
    int *voA = &voAval, *voB = &voBval;
    int instruccion;
    int mnemo, cantOperandos;
    instruccion = RAM[REG[5]];
    REG[5]++;
    decInstruccion(instruccion, &cantOperandos, &mnemo);
    traduceOperandos(instruccion, cantOperandos, &voA, &voB);
    vecFunciones[mnemo](voA, voB);
}

void iniciaEjecucion(FILE *arch, int *i)
{
    int vec[5];
    char mv[5] = "\0";
    //Si la primer linea guarda el valor mv21
    fread(&mv, sizeof(char) * 4, 1, arch);
    if (strcmp(mv, "MV21") == 0)
    {
        //Cargamos el tamaño de los cuatro registros
        *i += 1;
        //Cuando sale de este while *i==5 pero no leyo la instruccion 5
        while (*i <= 4 && fread(&vec[*i], sizeof(int), 1, arch) == 1)
        {
            if (vec[*i] >= 0 && vec[*i] <= 0xFFFF)
                *i += 1;
            else
                *i = 6; //Valor cualquiera para que salga del while (aunque distinto de 5)
        }
        if (*i == 5)
        {
            if (vec[1] + vec[2] + vec[3] <= (8192 - vec[4]))
            {
                //Code Segment CS
                setParteAlta(&REG[3], vec[4]);
                setParteBaja(&REG[3], 0);
                //Data Segment DS
                setParteAlta(&REG[0], vec[1]);
                setParteBaja(&REG[0], vec[4]); // No le sumo 1 porque el primero arranca en 0
                //Extra Segment ES
                setParteAlta(&REG[2], vec[3]);
                setParteBaja(&REG[2], vec[4] + vec[1]); //En la posicion que indique la suma de los tamaños anteriores
                //Stack Segment SS
                setParteAlta(&REG[1], vec[2]);
                setParteBaja(&REG[1], vec[4] + vec[1] + vec[3]); //En la posicion que indique la suma de los tamaños anteriores
                *i = 0;                                          //Ponemos el i en 0 para que comienze la lectura en la RAM desde cero

                //Inicializamos los registros restantes
                REG[4] = -1;
                for (int j = 5; j <= 15; j++)
                    REG[j] = 0;
                //SP lo inicialziamos con el tamaño de la pila en la parte baja, y en la alta con el codigo de segmento del SS
                setParteAlta(&REG[6], 1);
                setParteBaja(&REG[6], vec[2]);
                //BP lo inicializamos con cero en la parte baja, y en la alta con el codigo de segmento del SS
                setParteAlta(&REG[7], 1);
                setParteBaja(&REG[7], 0);
            }
            else
            {
                printf("Mal asignado el espacio de la ram\n");
                *i = -1;
            }
        }
        else
        {
            printf("Mal hecho el encabezado. Pocas lineas de encabezado o mal asignado el espacio de memoria a los registros\n");
            *i = -1;
        }
    }
    else
    {
        printf("El formato de archivo x.bin no es correcto\n");
        *i = -1;
    }
}

int devuelveIndirecto(int valor)
{
    int offset = (valor >> 4) & 0xFF;
    trunca(&offset, 8);
    short int codReg = valor & 0xF;
    return getPosicionAbsoluta(REG[codReg] + offset);
}

//PARA REGISTROS
int getPosicionAbsoluta(int valor)
{
    int highV = getParteAlta(valor);
    int lowV = getParteBaja(valor);
    int pos;
    pos = getParteBaja(REG[highV]) + lowV;
    if (pos >= getParteBaja(REG[highV]) && lowV <= getParteAlta(REG[highV]) && highV >= 0 && highV <= 3)
        return pos;
    else
    {
        printf("Segmentation Fault. Invalid memory reference. \n");
        exit(1);
    }
}

int getParteAlta(int valor)
{
    return (valor & 0xFFFF0000) >> 16;
}

int getParteBaja(int valor)
{
    return valor & 0xFFFF;
}

void setParteAlta(int *num, int val)
{
    val = (val & 0xFFFF) << 16;
    *num = *num & 0x0000FFFF;
    *num = *num | val;
}

void setParteBaja(int *num, int val)
{
    val = val & 0xFFFF;
    *num = *num & 0xFFFF0000;
    *num = *num | val;
}
