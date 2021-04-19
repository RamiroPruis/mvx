#include <stdio.h>
#include <stdlib.h>



int main(char *argv[])
{
    FILE* arch;
    int RAM[4096];
    int REG[16];
    int instruccion;
    int i=0,DS;

    if ((arch = fopen(argv[1], "r")) == NULL)
        return 1;
    while(fread(RAM[i],sizeof(int),arch)!=NULL)
      i++;
    DS = i-1;

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
