#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "info.h"

#define NOT_OK          0
#define TODO_OK         1
#define LISTA_VACIA     2
#define SIN_MEMORIA     3

typedef struct s_nodo{
    t_dato info;
    struct s_nodo *sig;
}t_nodo;

typedef t_nodo *t_list;

typedef int (*t_cmp)(const void*, const void*);

/** PRIMITIVAS */
void crearLista(t_list *);
int listaLlena(const t_list *);
int listaVacia(const t_list *);
int insertarAlFinal(t_list *, const t_dato *);
int eliminarPorClave(t_list *, const t_dato *, t_cmp);
int buscarEnListaNoOrdenadaPorClave(t_list *, t_dato *, t_cmp);
int mostrarLista(t_list *, char *, t_cmp);

/** FUNCIONES EXTRA*/

/*
    Compara las dos patentes enviadas, retorna:
    0: si no son iguales
    1: si son iguales
*/
int *compararPatente(const t_dato *d1, const t_dato *d2);

/*
    Compara los dos partidos enviados, retorna:
    0: si no son iguales
    1: si son iguales
*/
int *compararPartido(const t_dato *d1, const char *d2);

#endif // LIST_H_INCLUDED
