typedef struct
{
    char mnemo[10];
    int hex;
} Tvec;

typedef struct
{
    char cadena[100];
} Tdisasembler;

void traduceOperandos(int, int, int **, int **);
void decInstruccion(int, int *, int *);
void MOV(int *, int *);
void ADD(int *, int *);
void SUB(int *, int *);
void MUL(int *, int *);
void DIV(int *, int *);
void SWAP(int *, int *);
void CMP(int *, int *);
void SHL(int *, int *);
void SHR(int *, int *);
void NOT(int *, int *);
void STOP(int *, int *);
void JNN(int *, int *);
void JNP(int *, int *);
void JNZ(int *, int *);
void JN(int *, int *);
void JP(int *, int *);
void JZ(int *, int *);
void JMP(int *, int *);
void XOR(int *, int *);
void OR(int *, int *);
void AND(int *, int *);
void LDL(int *, int *);
void LDH(int *, int *);
void RND(int *, int *);
void SLEN(int *, int *);
void SMOV(int *, int *);
void SCMP(int *, int *);
void SYS(int *, int *);
void PUSH(int *, int *);
void POP(int *, int *);
void CALL(int *, int *);
void RET(int *, int *);

void creadicc(Tvec[]);
void creaReg(Tvec[]);
void (*vecFunciones[32])(int *, int *);
void cargaFunciones();
void pasoApaso();
void desarmaPalabra(char[], char[], char[]);
void muestraValor(char[]);
void pasoApaso(char[]);
void dissasembler(int, int);
void traduceIntruccion(char[], int, Tvec[], Tvec[]);
void BuscaRegistro(int, int *, Tvec[]);
void trunca(int *, int);
void proxinstruccion();
void iniciaEjecucion(FILE *, int *);
int getPosicionAbsoluta(int);
int devuelveIndirecto(int);
int getParteAlta(int);
int getParteBaja(int);
void setParteBaja(int *, int);
void setParteAlta(int *, int);