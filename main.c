#include <stdio.h>
#include <stdlib.h>
#include "decoinst.h"


int main(/*char *argv[]*/)
{

  int instruccion;
  int i = 0;
  int mnemo, cantOperandos;
  int *voA, *voB;


  *voA = 2;
  REG[0]=15;
  REG[10]= 0x100 | 0x10;
  RAM[REG[0]+1]='H';
  RAM[REG[0]+2]='o';
  RAM[REG[0]+3]='l';
  RAM[REG[0]+4]='a';
  RAM[REG[0]+5]='\0';
  REG[13]=1;
  REG[12]=4;

  SYS(voA,0);



  /*
    if ((arch = fopen(argv[1], "rb")) == NULL)
        return 1;
    while(fread(RAM[i],sizeof(int),1,arch)!=NULL)
      i++;
    REG[0] = i-1; //DS
    */

  REG[5] = 0; //IP
  /*
  while (REG[5] >= 0 && REG[5] < REG[0])
  {
    //Obtener proxima instruccion
    instruccion = RAM[REG[5]];
    REG[5]++;
    decInstruccion(instruccion, &cantOperandos, &mnemo);
    traduceOperandos(instruccion, cantOperandos, &voA, &voB);
    vecFunciones[mnemo](voA, voB); //Ejecuta
  }
  */

  return 0;
}
