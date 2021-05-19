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
int flagB = 0;
int flagC = 0;
int flagD = 0;
Tvec vecReg[10];
Tvec vecMnemo[25];

int main(/*int argc, char *argv[]*/)
{
  FILE *arch;
  //  int instruccion;
  int i = 0, j = 0;
  // int mnemo, cantOperandos;
  //  int voAval, voBval;
  //  int *voA = &voAval, *voB = &voBval;
  //size_t len = strlen(argv[1]);
  //const char *bin = &argv[1][len - 4];

  // if (argc<2)
  //   printf("Error. Faltan argumentos. Recomendacion:\n mvx.exe BinFilename [-b] [-c] [-d] (flags opcionales[]) \n");
  // else
  //   if (strcmp(bin,".bin")!=0) //SI LA EXTENSION NO ES .BIN
  //       printf("Error. El archivo binario no es de tipo .bin \n");
  //   else
  //       if (argc>2){
  //           j=3;
  //           while (j<=argc){
  //               if (strcmp(argv[j-1],"-b")==0)
  //                   flagB=1;
  //               else
  //                   if (strcmp(argv[j-1],"-c")==0)
  //                       flagC=1;

  //                   else
  //                       flagD=1;
  //               j++;
  //           }
  //       }
  flagD = 1;
  flagB = 1;
  if ((arch = fopen("5.bin", "rb")) == NULL)
    return 1;

  //Encabezado
  iniciaEjecucion(arch, &i);
  if (i == 0)
  {
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
  }
  //Si hubo algun problema con el encabezado no se ejecuta el programa
  else
    return 1;

  REG[5] = 0; //IP
  if (flagD)
  {
    //mostramos por primera vez
    for (int i = 0; i < REG[0]; i++)
      printf("%s\n", DISASEMBLER[i].cadena);
    printf("\n");
  }

  cargaFunciones();
  RAM[999] = 0;
  while (REG[5] >= 0 && REG[5] < REG[0])
  {
    proxinstruccion();
  }

  return 0;
}
