#include "decoinst.h"

void decInstruccion(int instruccion){
  int codigo;
  int cantOperando;

  if ((instruccion >> 24) == 0xFF){
    //codigo 0 op
    cantOperando = 0;
    codigo = instruccion >> 20;
  }
  else
    if ((instruccion >> 28) == 0xF){
      //codigo 1op
      cantOperando = 1;
      codigo = instruccion >> 24;
    }
    else{
      //codigo 2op
      cantOperando = 2;
      codigo = instruccion >> 28;
    }
}

void cambiaCC(int val){
  if (val == 0)
      REG[8] = 0x80000000;
    else
      if (val < 0)
        REG[8] = 1;
      else
        REG[8] = 0;

}

//OPERACIONES
void MOV(int *valA,int *valB){

  *valA = *valB;

}

void ADD(int *valA,int *valB){

  *valA = *valA + *valB;
  cambiaCC(*valA);


}

void SUB(int *valA,int *valB){

  *valA = *valA - *valB;

  cambiaCC(*valA);

}

void MUL(int *valA,int *valB){

  *valA = *valA * (*valB);

  cambiaCC(*valA);

}

void DIV(int *valA,int *valB){

  *valA = (int) *valA / (*valB);
  REG[9] = *valA % (*valB);
  cambiaCC(*valA);

}









