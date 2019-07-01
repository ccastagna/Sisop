// Script Tp3-Ej2

// Trabajo practico 3
// Ejercicio 2
// Entrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869
#ifndef INFO_H_INCLUDED
#define INFO_H_INCLUDED
#include <stdio.h>
#include <pthread.h>
#define MAESTRO_REGISTRY_SIZE 54
#define STOCK_REGISTRY_SIZE 14

typedef struct{
    char id[8];
    char description[40];
    char stockMinimo[5];
}t_maestro;

typedef struct{
    char id[8];
    char stock[5];
}t_stock;

typedef struct{
    char * filename;
}t_file;

typedef struct{
    FILE * log;
    int max_position;
    unsigned int * ids_array;
    unsigned int * real_stock_array;
    pthread_t tid;
    int       threadNum;
}t_thread;
#endif // INFO_H_INCLUDED
