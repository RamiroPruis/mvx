#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decoinst.h"

extern int RAM[4096];
extern int REG[16];
extern int *voAStatic;
extern int *voBStatic;
extern int flagB;
extern int flagC;
extern int flagD;
extern Tdisasembler DISASEMBLER[1000];
extern Tvec vecReg[10];
extern Tvec vecMnemo[25];

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
}

void creaReg(Tvec registros[])
{
  //Inicializamos en null
  for (int i = 0; i < 16; i++)
    strcpy(registros[i].mnemo, "\0");

  strcpy(registros[0].mnemo, "DS");
  registros[0].hex = 0;
  strcpy(registros[5].mnemo, "IP");
  registros[5].hex = 5;
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
        printf(prompt, REG[13] + i);
        scanf(cad, &RAM[REG[0] + REG[13] + i]);
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
            printf("%s\n", DISASEMBLER[i]);
          }
          else
            printf(" %s\n", DISASEMBLER[i]);

      else
        for (int i = 0; i < 10; i++)
          if (i == REG[5])
          {
            printf(">");
            printf("%s\n", DISASEMBLER[i]);
          }
          else
            printf(" %s\n", DISASEMBLER[i]);

      //proxinstruccion();
      sprintf(cad, "Registros:\n");
      int j = 0;
      for (i = 1; i <= 4; i++)
      {
        while (j < i * 4)
        {
          if (vecReg[j].mnemo[0] != '\0')
            sprintf(cad2, "%s = %15d |", vecReg[j].mnemo, REG[j]);
          else
            sprintf(cad2, "%20s |", vecReg[j].mnemo);
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

void dissasembler(int instruccion, int i)
{
  char cad1[100] = {'\0'};
  char cad2[100] = {'\0'};

  sprintf(cad1, "[%04d]: %02X %02X %02X %02X   %d: ", i, (instruccion >> 24) & 0xFF, (instruccion >> 16) & 0xFF, (instruccion >> 8) & 0xFF, (instruccion >> 0) & 0xFF, i + 1);
  traduceIntruccion(cad2, instruccion, vecMnemo, vecReg); //Retorna en cad2 el "MOV ...."
  strcat(cad1, cad2);                                     //Queda en cad 1 toda la expresion a guardar
  strcpy(DISASEMBLER[i].cadena, cad1);
}

void traduceIntruccion(char cad[], int inst, Tvec cod[], Tvec reg[])
{
  int i, j;
  char op1[5] = "\0";
  int aver1 = (inst >> 28) & 0xF;
  int aver2 = cod[11].hex;

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
      while (cod[i].hex != ((inst >> 24) & 0xFF))
        i++;
      strcpy(cad, cod[i].mnemo);
      if (((inst >> 22) & 0x003) == 0x000)
      { // operando inmediato
        sprintf(op1, " %d", (inst & 0x000FFFFF));
        strcat(cad, op1);
      }
      else if (((inst >> 22) & 0x003) == 0x001)
      { // operando de registro
        j = 0;
        while (reg[j].hex != (inst & 0x000FFFFF))
          j++;
        sprintf(op1, " %s", reg[j].mnemo);
        strcat(cad, op1);
      }
      else if (((inst >> 22) & 0x003) == 0x002)
      { // operando directo
        sprintf(op1, " [%d]", (inst & 0x000FFFFF));
        strcat(cad, op1);
      }
    }
    else
    { // 2 operandos
      i = 0;
      while ((cod[i].hex) != ((inst >> 28) & 0xF))
        i++;
      strcpy(cad, cod[i].mnemo);
      if (((inst >> 26) & 0x03) == 0x00)
      { // operando 1 inmediato
        sprintf(op1, " %d,", ((inst >> 12) & 0x00FFF));
        strcat(cad, op1);
      }
      else if (((inst >> 26) & 0x03) == 0x01)
      { // operando 1 de registro
        j = 0;
        while (reg[j].hex != ((inst >> 12) & 0x00FFF))
          j++;
        sprintf(op1, " %s,", reg[j].mnemo);
        strcat(cad, op1);
      }
      else if (((inst >> 26) & 0x03) == 0x02)
      { // operando 1 directo
        sprintf(op1, " [%d],", ((inst >> 12) & 0x00FFF));
        strcat(cad, op1);
      }

      if (((inst >> 24) & 0x03) == 0x00)
      { // operando 2 inmediato
        sprintf(op1, "%d", (inst & 0x00000FFF));
        strcat(cad, op1);
      }
      else if (((inst >> 24) & 0x03) == 0x01)
      { // operando 2 de registro
        j = 0;
        while (reg[j].hex != (inst & 0x00000FFF))
          j++;
        sprintf(op1, "%s", reg[j].mnemo);
        strcat(cad, op1);
      }
      else if (((inst >> 24) & 0x03) == 0x02)
      { // operando 2 directo
        sprintf(op1, "[%d]", (inst & 0x00000FFF));
        strcat(cad, op1);
      }
    }
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
