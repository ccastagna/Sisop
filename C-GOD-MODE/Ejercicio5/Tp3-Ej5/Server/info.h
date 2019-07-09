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

typedef struct{
    t_dato multas[MAX_BUFFER_MULTAS];
    int cantMultas;
    char *msg;
    int opcion;
}t_buffer;

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
}arg;


#endif // INFO_H_INCLUDED
