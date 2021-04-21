#include <stdio.h>
#include <stdlib.h>
#include "decoinst.h"

void prueba();

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