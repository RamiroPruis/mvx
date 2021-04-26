#include <stdio.h>
#include <stdlib.h>
#include "decoinst.h"
#include <string.h>

int RAM[4096];
int REG[16];
int voAStaticVal, voBStaticVal;
int *voAStatic = &voAStaticVal;
int *voBStatic = &voBStaticVal;
Tdisasembler DISASEMBLER[1000];
int flagB=0;
int flagC=0;
int flagD=0;
Tvec vecReg[10];
Tvec vecMnemo[25];

int main(int argc, char *argv[])
{
  FILE *arch;
//  int instruccion;
  int i=0,j=0;
// int mnemo, cantOperandos;
//  int voAval, voBval;
//  int *voA = &voAval, *voB = &voBval;
  size_t len=strlen(argv[1]);
  const char *bin=&argv[1][len-4];

  if (argc<2)
    printf("Error. Faltan argumentos. Recomendacion:\n mvx.exe BinFilename [-b] [-c] [-d] (flags opcionales[]) \n");
  else
    if (strcmp(bin,".bin")!=0) //SI LA EXTENSION NO ES .BIN
        printf("Error. El archivo binario no es de tipo .bin \n");
    else
        if (argc>2){
            j=3;
            while (j<=argc){
                if (strcmp(argv[j-1],"-b")==0)
                    flagB=1;
                else
                    if (strcmp(argv[j-1],"-c")==0)
                        flagC=1;

                    else
                        flagD=1;
                j++;
            }
        }
  if ((arch = fopen(argv[1], "rb")) == NULL)
    return 1;
  creadicc(vecMnemo);
  creaReg(vecReg);
  if (!flagD)
    while (fread(&RAM[i], sizeof(int), 1, arch) == 1)
      i++;
  else
  {
    printf("Codigo:\n");
    while (fread(&RAM[i], sizeof(int), 1, arch) == 1)
    {
      dissasembler(RAM[i], i);
      i++;
    }
  }

  REG[0] = i; //DS apunta a la ultima linea de binario a interpretar

  REG[5] = 0; //IP

  if (flagD)
  {
    //mostramos por primera vez
    for (int i = 0; i < REG[0]; i++)
      printf("%s\n", DISASEMBLER[i].cadena);
    printf("\n");
  }

  cargaFunciones();

  while (REG[5] >= 0 && REG[5] < REG[0])
  {
    // Obtener proxima instruccion
    // instruccion = RAM[REG[5]];
    // REG[5]++;
    // decInstruccion(instruccion, &cantOperandos, &mnemo);
    // traduceOperandos(instruccion, cantOperandos, &voA, &voB);
    // //printf("[%04d]: %02X %02X %02X %02X\n", REG[5], (instruccion >> 24) & 0xFF, (instruccion >> 16) & 0xFF, (instruccion >> 8) & 0xFF, (instruccion >> 0) & 0xFF);
    // vecFunciones[mnemo](voA, voB); //Ejecuta
    proxinstruccion();
  }
  return 0;
}
