#ifndef INFO_H_INCLUDED
#define INFO_H_INCLUDED
#include <stdio.h>

typedef struct{
    char id[8];
    char description[40];
    char stockMinimo[5];
}t_maestro;

typedef struct{
    t_maestro dato;
    long stockActual;
}t_dato;

typedef struct{
    char id[8];
    char stock[5];
}t_stock;

typedef struct{
    char * filename;
}t_file;
#endif // INFO_H_INCLUDED
