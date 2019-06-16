#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <string.h>
#include <stdlib.h>

#include "info.h"
#include "list.h"

#define NO_EXISTE_PATENTE   0
#define EXISTE_PATENTE      1

#define TODO_OK 0
#define NOT_OK  1

// Usado para controlar que registros debo eliminar de la base de datos.
#define NO_CAMBIO   0
#define CAMBIO      1
#define ELIMINAR    2

// Usado para archivos
#define DATABASE_NAME       "infractores.txt"
#define READ_TEXT           "rt"  //Abre un fichero de texto para lectura
#define READ_TEXT_UPDATE    "r+t" //Abre un fichero de texto para actualización (lectura y escritura)
#define CON_MSG             1


int abrirArchivo(FILE **, const char*, const char*, int);

int leerArchivo(FILE **, t_list *pl, char *);

//escribirArchivo();

/*
    Recibe patente y el monto de la nueva multa. Si existe suma monto al total y aumenta
    cantidad de multas, sino existe crea un nuevo registro en la base de datos.
*/
int ingresarMulta(char*, double, t_list *);

/*
    Recibe la patente y devuelve si existe o no.
*/
int existePatente(char*, t_list *);

/*
    Buscar lista de registros a suspender. Los registros a suspender son aquellos
    de las personas que deben un monto total mayor a $20.000 y/o que poseen más de 3 multas.
    Retorna una lista de ellos.
*/
int registrosSuspender(t_list *);

/*
    Salda la deuda de la patente recibida, es decir lo elimina de la base de datos.
*/
int saldarMulta(char*, t_list *);

/*
    Busca el monto total a pagar de la patente recibida.
*/
double buscarMontoTotal(char*, t_list*);

/*
    Muestra el monto total a pagar de cada infractor
*/
int verMontoTotalInfractores(t_list *);


#endif // FUNCTIONS_H_INCLUDED
