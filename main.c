#include <stdio.h>
#include <stdlib.h>
#include "decoinst.h"

int RAM[4096];
int REG[16];

void prueba();

int main(/*char *argv[]*/)
{

    int instruccion;
    int i=0,DS;

    /*
    if ((arch = fopen(argv[1], "rb")) == NULL)
        return 1;
    while(fread(RAM[i],sizeof(int),1,arch)!=NULL)
      i++;
    DS = i-1;
    */

    REG[5] = 0; //IP

    while(REG[5]>=0 && REG[5]<DS){
      //Obtener proxima instruccion
      instruccion = RAM[REG[5]];
      REG[5]++;
      //Decodificar instruccion
      //Decodificar operandos
      //Ejecutar instruccion

    }

    return 0;
}

void prueba(){
  REG[5] = 10;
}


