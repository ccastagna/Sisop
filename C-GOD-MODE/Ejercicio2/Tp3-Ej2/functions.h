
#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "listaDinamica.h"
#include "colaDinamica.h"

t_lista lista;
t_cola cola;

int isNumber(char *number);
int cargoArchivoMaestro(char *fname);
int cargoArchivoStock(char *fname);
//int createOutputFiles(FILE **compras,FILE ** stock, FILE **log);
int procesarArchivos(int cantThreads, char * path);


#endif // FUNCTIONS_H_INCLUDED
