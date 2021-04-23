#include <stdio.h>
#include <stdlib.h>
#include "decoinst.h"

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
  regFlags.flagB = 1;
  regFlags.flagC = fgC;
  regFlags.flagD = fgD;
  while (REG[5] >= 0 && REG[5] < REG[0])
  {
    //Obtener proxima instruccion
    instruccion = RAM[REG[5]];
    REG[5]++;
    decInstruccion(instruccion, &cantOperandos, &mnemo);
    traduceOperandos(instruccion, cantOperandos, &voA, &voB);
    printf("[%04d]: %02X %02X %02X %02X\n", REG[5], (instruccion >> 24) & 0xFF, (instruccion >> 16) & 0xFF, (instruccion >> 8) & 0xFF, (instruccion >> 0) & 0xFF);
    vecFunciones[mnemo](voA, voB); //Ejecuta
  }

  return 0;
}