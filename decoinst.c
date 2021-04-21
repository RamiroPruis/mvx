#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decoinst.h"

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