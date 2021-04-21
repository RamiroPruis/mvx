#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decoinst.h"

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
      *voA = (int **)malloc(sizeof(int *));
      **voA = voAux;
    }

    else if (toA == 0x01)
      **voA = voAux;

    else
      **voA = RAM[(voAux + REG[0])];

    //      B
    if (toB == 0x00)
    {
      *voB = (int **)malloc(sizeof(int *));
      **voB = voBux;
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
      *voA = (int **)malloc(sizeof(int *));
      **voA = voAux;
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
  //vecFunciones[12] = SYS;
  vecFunciones[13] = JMP;
  vecFunciones[14] = JZ;
  vecFunciones[15] = JP;
  vecFunciones[16] = JN;
  vecFunciones[17] = JNZ;
  vecFunciones[18] = JNP;
  vecFunciones[19] = JNN;
  //vecFunciones[20] = LDL;
  //vecFunciones[21] = LDH;
  //vecFunciones[22] = RND;
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
}

void JNN(int *valA, int *valB)
{
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

void SYS(int *valA, int *valB)
{

  char aux;
  char *vec = &aux;
  int idInicia;
  int i = 1;
  //Ahora reservamos memoria dinamica del vector dependiendo CX==reg[6], acordarse de al final hacer un free
  vec = (char *)malloc(sizeof(char) * REG[6]);
  //Caso LECTURA
  if (*valA == 1)
  {
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
    if (!((REG[4] >> 8) & 0x00000001))
    {
      idInicia = REG[7];
      //Debemos interpretar (hexa,oct,decimal)
      //Hexa
      if ((REG[4] >> 3) & 0x00000001)
      {
        for (int i = 0; i < REG[6]; i++)
        {
          RAM[idInicia] = strtol(*vec, NULL, 16);
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
  }

  //Caso ESCRITURA
  else if (*valA == 1)
  {
    //Primero vemos que interpretacion tenemos que guardar en el vector y luego imprimimos
    //Byte menos significativo como CARACTER
    if ((REG[4] >> 4) & 0x00000001)
    {
    }
    //Hexa
    else if ((REG[4] >> 3) & 0x00000001)
    {
    }
    //Octal
    else if ((REG[4] >> 2) & 0x00000001)
    {
    }
    //Decimal
    else if (REG[4] & 0x00000001)
    {
    }
    else
      printf("Error, no indica como imprimir los caracteres\n"); //Deberiamos poner una bandera para ver si se pudo interpretar bien y comenzar con la escritura

    //Si se pudo interpretar
    if (algo)
    {
      idInicia = REG[7];
      //Vemos si es con o sin prompt
      if (!((REG[4] >> 11) & 0x00000001)) //Prompt
      {
        //Vemos si es con endline
        if (!((REG[4] >> 8) & 0x00000001))
          for (int i = 0; i < REG[6]; i++)
          {
            printf("[%04d]: %s\n", idInicia, vec);
            vec++;
            idInicia++;
          }
        else
        { //Sin endline
          for (int i = 0; i < REG[6]; i++)
          {
            printf("[%04d]: %s", idInicia, vec);
            vec++;
            idInicia++;
          }
          printf("\n");
        }
      }
      else
      { //Sin prompt
        //Vemos si es con endline
        if (!((REG[4] >> 8) & 0x00000001))
        {
          for (int i = 0; i < REG[6]; i++)
          {
            printf("%s\n", vec);
            vec++;
            idInicia++;
          }
        }
        else
        { //Sin endline
          for (int i = 0; i < REG[6]; i++)
          {
            printf("%s", vec);
            vec++;
            idInicia++;
          }
          printf("\n");
        }
      }
    }
  }
  else
    printf("Error, SYS no interpreta ese argumento\n");
  free(vec); //Para todos los casos
}
