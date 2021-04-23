#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int RAM[4096] = {0};
int REG[16];
int voAStaticVal, voBStaticVal;
int *voAStatic = &voAStaticVal;
int *voBStatic = &voBStaticVal;

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

int main(/*int argc, char *argv[]*/)
{
    FILE *arch;
    int instruccion;
    int i = 0;
    int mnemo, cantOperandos;
    int fgB = 0, fgC = 0, fgD = 0;
    int voAval, voBval;
    int *voA = &voAval, *voB = &voBval;
    //int len=strlen(argv[1]);
    //const char *bin=&argv[1][len-4];

    // *voA = 2;
    // REG[0] = 15;
    // REG[10] =  0x10;
    // RAM[REG[0] + 1] = 30;
    // RAM[REG[0] + 2] = 'o';
    // RAM[REG[0] + 3] = 'l';
    // RAM[REG[0] + 4] = 'a';
    // RAM[REG[0] + 5] = '\0';
    // REG[13] = 1;
    // REG[12] = 4;

    // SYS(voA, 0);

    /*
  if (argc<2)
    printf("Error. Faltan argumentos. Recomendacion:\n
           mvx.exe BinFilename [-b] [-c] [-d] (flags opcionales[])");
  else
    if (strcmp(argv[1],bin)!=0){ //SI LA EXTENSION NO ES .BIN
        printf("Error. El archivo binario no es de tipo .bin");
    else
        if (argc>2){
            i=3;
            while (i<=argc){
                switch argv[i-1]:
                case "-b":
                    fgB=1;
                    break;
                case "-c":
                    fgC=1;
                    break;
                case "-d":
                    fgD=1;
                    break;
            }
        }

  */

    if ((arch = fopen("binardo.bin", "rb")) == NULL)
        return 1;
    while (fread(&RAM[i], sizeof(int), 1, arch) == 1)
        i++;
    REG[0] = i - 1; //DS apunta a la ultima linea de binario a interpretar

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
        voBux = instruccion & 0x00000FFF;
        //       A
        if (toA == 0x00)
        {
            *voAStatic = voAux;
            *voA = voAStatic;
        }
        else if (toA == 0x01)
            *voA = &REG[voAux];
        else
            *voA = &RAM[(voAux + REG[0])];

        //      B
        if (toB == 0x00)
        {
            *voBStatic = voBux;
            *voB = voBStatic;
        }
        else if (toB == 0x01)
            *voB = &REG[voBux];
        else
            *voB = &RAM[(voBux + REG[0])];
    }
    else if (cantOperandos == 1)
    {
        toA = (instruccion >> 22) & 0x00000003;
        voAux = instruccion & 0x0000FFFF;
        //       A
        if (toA == 0x00)
        {
            *voAStatic = voAux;
            *voA = voAStatic;
        }
        else if (toA == 0x01)
            *voA = &voAux;

        else
            *voA = &RAM[(voAux + REG[0])];
    }
}

void decInstruccion(int instruccion, int *cantOperando, int *codigo)
{
    int que = (instruccion >> 24) & 0xFF;

    if (((instruccion >> 24) & 0xFF) == 0xFF)
    {
        //codigo 0 op
        *cantOperando = 0;
        *codigo = (instruccion >> 20) & 0xFFF;
        *codigo = *codigo - 4056; //Para el vector Funciones
    }
    else if (((instruccion >> 28) & 0xF) == 0xF)
    {
        //codigo 1op
        *cantOperando = 1;
        *codigo = (instruccion >> 24) & 0xFF;
        *codigo = *codigo - 228;
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
        REG[8] = 1; //preguntar
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
    char prompt[10] = "";
    char cad[100] = {"\0"};
    char cad2[100] = {""};
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
                strcat(cad, "%x ");
            if ((REG[10] & 0x4) != 0)
                strcat(cad, "%o ");
            if ((REG[10] & 0x1) != 0)
                strcat(cad, "%d ");
        }

        if (condChar == 0)
            for (i = 0; i < REG[12]; i++)
            {
                printf(prompt, REG[13] + i);
                scanf(cad, &(RAM[REG[0] + REG[13] + i]));
            }
        else
        {
            printf(prompt, REG[13]);
            scanf("%s", entrada);
            for (i = 0; entrada[i]; i++)
                RAM[REG[0] + REG[13] + i] = entrada[i];
        }
    }
    //Caso ESCRITURA
    else if (*valA == 2)
    {
        if ((REG[10] & 0x10) != 0)
            strcat(cad2, "%c");
        if ((REG[10] & 0x8) != 0)
            strcat(cad, "%X");
        if ((REG[10] & 0x4) != 0)
            strcat(cad, "%o");
        if ((REG[10] & 0x1) != 0)
            strcat(cad, "%d");
        if ((REG[10] & 0x100) == 0)
            strcat(cad, "\n");

        for (i = 0; i < REG[12]; i++)
        {

            printf(prompt, REG[13] + i);
            if (RAM[REG[0] + REG[13] + i] >= 32)
                caracter = RAM[REG[0] + REG[13] + i];
            else
                caracter = '.';
            printf(cad2, caracter);
            printf(cad, RAM[REG[0] + REG[13] + i], RAM[REG[0] + REG[13] + i], RAM[REG[0] + REG[13] + i]);
        }
    }
    // else if (*valA == 15)
    // { //F
    //     //Con los flags, podriamos hacer un registro
    //     //y pasarle como parametro al sys ese registro
    //     if (fgA)
    //     {
    //         printf("%s cmd:", prompt);
    //         scanf("");
    //     }
    // }
    else
        printf("Error, SYS no interpreta ese argumento\n");
}
