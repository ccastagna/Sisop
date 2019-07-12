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

#ifndef INFO_H_INCLUDED
#define INFO_H_INCLUDED

#define MAX_BUFFER_MULTAS     2000

typedef struct {
    char *partido;
    char *patente;
    char *nombre_titular;
    int cantidad_multas;
    float monto_total;
} t_dato;

typedef struct {
    char partido[20];
    char patente[8];
    char nombre_titular[25];
    int cantidad_multas;
    float monto_total;
} t_dato_buffer;

typedef struct{
    t_dato_buffer multas[MAX_BUFFER_MULTAS];
    int cantMultas;
    char msg[200];
    int opcion;
}t_buffer;

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
}arg;


#endif // INFO_H_INCLUDED
