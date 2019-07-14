// Script Tp3-Ej2

// Trabajo practico 3
// Ejercicio 2
// Primera Reentrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "colaDinamica.h"

t_cola cola;

int isNumber(char *number);
int cargoArchivoMaestro(char *fname, unsigned int * ids_array, unsigned int * min_stock_array, unsigned int * real_stock_array);
int cargoArchivoStock(char *fname);
int calcularCantRegistros(char *fname);
//int createOutputFiles(FILE **compras,FILE ** stock, FILE **log);
int procesarArchivos(int cantThreads, char * path, unsigned int * ids_array, unsigned int * min_stock_array, unsigned int * real_stock_array, int max_position);

void mostrarArray(unsigned int * array, int max_position);
#endif // FUNCTIONS_H_INCLUDED
