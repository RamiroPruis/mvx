
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




