#ifndef INFO_H_INCLUDED
#define INFO_H_INCLUDED

typedef struct {
    char *partido;
    char *patente;
    char *nombre_titular;
    int cantidad_multas;
    float monto_total;
} t_dato;

typedef struct{
    t_dato multas[2000];
    int cantMultas;
    char *msg;
    int opcion;
}t_buffer;


#endif // INFO_H_INCLUDED
