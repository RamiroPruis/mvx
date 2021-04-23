typedef struct
{
    int flagB, flagC, flagD;
} TregFlags;

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
void SYS(int *, int *);

void (*vecFunciones[25])(int *, int *);
void cargaFunciones();
void pasoApaso();
void desarmaPalabra(char[], char[], char[]);
void muestraValor(char[]);
void pasoApaso(char[]);
