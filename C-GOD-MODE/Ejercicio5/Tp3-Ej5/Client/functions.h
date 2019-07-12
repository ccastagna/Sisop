// Script Tp3-Ej5-Client

// Trabajo Practico 3
// Ejercicio 5
// Primera Reentrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869


#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../Server/info.h"

#define NO_EXISTE_PATENTE   0
#define EXISTE_PATENTE      1

#define NOT_OK  0
#define TODO_OK 1

void mostrarMenu();

int normalizarCadena(unsigned char *, int);

void limpiarBuffer(t_buffer *);

int obtenerSemaforo(key_t );

void pedirSemaforo(int);

void devolverSemaforo(int );

void mostrarAyuda(char *);

#endif // FUNCTIONS_H_INCLUDED
