// Script Tp3-Ej3

// Trabajo practico 3
// Ejercicio 3
// Primera Reentrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869

#ifndef COLADINAMICA_H_INCLUDED
#define COLADINAMICA_H_INCLUDED

#include "info.h"
#include <stdio.h>
#include <stdlib.h>

#define SIN_MEMORIA 0
#define ACOLADO 1

#define COLA_VACIA 0
#define DESACOLADO 1

#define FALSO 0
#define VERDADERO 1

#define MOSTRAR_DATO 1

typedef struct s_nodoCD{
    struct s_nodoCD *sig;
    t_dato dato;

}t_nodoCD;

typedef struct{
    t_nodoCD *pri;
    t_nodoCD *ult;

}t_cola;

void crearCola(t_cola *cola);
int acolar(t_cola *cola,const t_dato *dato);
int desacolar(t_cola *cola,t_dato *dato);
void vaciarCola(t_cola *cola);
int colaVacia(const t_cola *cola);
int colaLlena(const t_cola *cola);
int verPrimero(const t_cola *cola, t_dato *dato);


#endif // COLADINAMICA_H_INCLUDED
