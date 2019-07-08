#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

// Script Tp3-Ej4-Cliente

// Trabajo práctico 3
// Ejercicio 4
// Primera Reentrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869

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
 * Muestra la ayuda para ejecutar el cliente.
 */
void mostrarAyuda(char *);

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
