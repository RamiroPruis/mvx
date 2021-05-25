#include <stdio.h>
#include <stdlib.h>
#include "decoinst.h"
#include <string.h>

int RAM[8192];
int REG[16];
int voAStaticVal, voBStaticVal;
int *voAStatic = &voAStaticVal;
int *voBStatic = &voBStaticVal;
Tdisasembler DISASEMBLER[1000];
int flagB = 0;
int flagC = 0;
int flagD = 0;
Tvec vecReg[16];
Tvec vecMnemo[32];

int main(int argc, char *argv[])
{
  FILE *arch;
  //  int instruccion;
  int i = 0, j = 0;
/*  size_t len = strlen(argv[1]);
  const char *bin = &argv[1][len - 4];

  if (argc < 2)
    printf("Error. Faltan argumentos. Recomendacion:\n mvx.exe BinFilename [-b] [-c] [-d] (flags opcionales[]) \n");
  else if (strcmp(bin, ".bin") != 0) //SI LA EXTENSION NO ES .BIN
    printf("Error. El archivo binario no es de tipo .bin \n");
  else if (argc > 2)
  {
    j = 3;
    while (j <= argc)
    {
      if (strcmp(argv[j - 1], "-b") == 0)
        flagB = 1;
      else if (strcmp(argv[j - 1], "-c") == 0){
        flagC=1;
        system("clear");
      }
      else
        flagD = 1;
      j++;
    }
  }

  if ((arch = fopen(argv[1], "rb")) == NULL)
    return 1;
*/
  if ((arch = fopen("prueba2.bin", "rb")) == NULL)
    return 1;
  flagD=1;
  //Encabezado
  cargaFunciones();
  iniciaEjecucion(arch, &i);
  if (i == 0)
  {
    creadicc(vecMnemo);
    creaReg(vecReg);
    while (fread(&RAM[i], sizeof(int), 1, arch) == 1)
      i++;
    if (flagD)
    {
      printf("Codigo:\n");
      j = 0;
      while (j < i)
      {
        dissasembler(RAM[j], &j);
        j++;
      }
    }
  }
  //Si hubo algun problema con el encabezado no se ejecuta el programa
  else
    return 1;

  int ds = getParteBaja(REG[0]);
  int cs = getParteBaja(REG[3]);
  REG[5] = getParteBaja(REG[3]); //IP INICALIZADO EN CS

  if (flagD)
  {
    //mostramos por primera vez
    for (int i = 0; i < ds; i++)
      if (DISASEMBLER[i].cadena[0])
        printf("%s\n", DISASEMBLER[i].cadena);
    printf("\n");
  }

  while (REG[5] >= cs && REG[5] < ds)
  {
    printf("%s\n", DISASEMBLER[REG[5]].cadena);
    proxinstruccion();
  }

  printf("Ejecucion Terminada. Gracias por utilizar MV21 -  ® Copyright © 2021 Grupo D MV Inc.");

  return 0;
}
