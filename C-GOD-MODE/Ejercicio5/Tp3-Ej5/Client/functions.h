#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <string.h>
#include <stdlib.h>

#include "../Server/info.h"

#define NO_EXISTE_PATENTE   0
#define EXISTE_PATENTE      1

#define NOT_OK  0
#define TODO_OK 1

void mostrarMenu();

int normalizarCadena(unsigned char *, int);

void limpiarBuffer(struct t_buffer *);

#endif // FUNCTIONS_H_INCLUDED
