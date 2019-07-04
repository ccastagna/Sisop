#ifndef INFO_H_INCLUDED
#define INFO_H_INCLUDED

typedef struct {
    char *partido;
    char *patente;
    char *nombre_titular;
    int cantidad_multas;
    float monto_total;
} t_dato;

typedef int (t_cmp)(const t_dato*, const t_dato*);

#endif // INFO_H_INCLUDED
