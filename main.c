#include <stdio.h>
#include <stdlib.h>
#include "decoinst.h"



void prueba();

int main(/*char *argv[]*/)
{

    int instruccion;
    int i=0;

    /*
    if ((arch = fopen(argv[1], "rb")) == NULL)
        return 1;
    while(fread(RAM[i],sizeof(int),1,arch)!=NULL)
      i++;
    REG[0] = i-1; //DS
    */

    REG[5] = 0; //IP


    while(REG[5]>=0 && REG[5]<REG[0]){
      //Obtener proxima instruccion
      instruccion = RAM[REG[5]];
      REG[5]++;
      //Decodificar instruccion
      //Decodificar operandos
      //Ejecutar instruccion

    }


    return 0;
}






