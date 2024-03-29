// Script Tp3-Ej5-Server

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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> 

#include "info.h"
#include "list.h"

#define NO_EXISTE_PATENTE   0
#define EXISTE_PATENTE      1

#define NOT_OK  0
#define TODO_OK 1

// Usado para archivos
#define DATABASE_NAME       "infractores.txt"
#define READ_TEXT           "rt"    //Abre un fichero de texto para lectura.
#define READ_TEXT_UPDATE    "r+t"   //Abre un fichero de texto para actualizaci�n (lectura y escritura).
#define WRITE_TEXT          "wt"    //Crea o pisa el contenido de un archivo.
#define CON_MSG             1

int abrirArchivo(FILE **, const char*, const char*, int);

int leerArchivo(FILE **, t_list *);

int escribirArchivo(FILE **, t_list *);

/*
    Recibe patente y el monto de la nueva multa. Si existe suma monto al total y aumenta
    cantidad de multas, sino existe crea un nuevo registro en la base de datos.
*/
int ingresarMulta(char*, char*, float, char*, t_list *);

/*
    Recibe la patente y devuelve si existe o no.
*/
int existePatente(t_dato*, t_list *);

/*
    Buscar lista de registros a suspender. Los registros a suspender son aquellos
    de las personas que deben un monto total mayor a $20.000 y/o que poseen m�s de 3 multas.
    Retorna una lista de ellos.
*/
int registrosSuspender(t_buffer *, t_list *, char*);

/*
    Salda la deuda de la patente recibida, es decir lo elimina de la base de datos.
*/
int saldarMulta( char*, char*, t_list *);

/*
    Busca el monto total a pagar de la patente recibida.
*/
int buscarMontoTotal(t_buffer *, char*, char*, t_list*);

/*
    Muestra el monto total a pagar de cada infractor
*/
int verMontoTotalInfractores(t_buffer *, t_list*, char*);

int obtenerSemaforo(key_t , int);

void pedirSemaforo(int );

void devolverSemaforo(int );

void eliminarSemaforo(int );

void mostrarAyuda(char *);

#endif // FUNCTIONS_H_INCLUDED
