#ifndef INFO_H_INCLUDED
#define INFO_H_INCLUDED

typedef struct {
    char *partido;
    char *patente;
    char *nombre_titular;
    int cantidad_multas;
    double monto_total;
    int cambio_registro; // 0: no cambio, 1: actualizar, 2: eliminar
} t_dato;


#endif // INFO_H_INCLUDED
