#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

// Bibliotecas de sistema
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// Bibliotecas propias
#include "info.h"

/*
 * Funcion utilizada para escribir mensajes al cliente.
 * Primero se le envia una cabecera indicandole que tipo de mensaje le sera enviado.
 * Luego le envia el cuerpo del mensaje al cliente.
 */
void escribirMensaje (int, int, char *, int);

/*
 * Funcion utilizada para leer los mensajes recibidos.
 */
void leerMensaje (int , int *,  char *);


#endif // FUNCTIONS_H_INCLUDED
