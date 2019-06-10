#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>

#define FALSO -1
#define TODO_OK 0
#define SIN_MEMORIA 1
#define DUPLICADO 2
#define LISTA_VACIA 3

typedef struct s_nodo{
    t_dato info;
    struct s_nodo *sig;
}t_nodo;


typedef t_nodo *t_list;
typedef int (*t_cmp) (const void *, const void *);

void crearLista(t_list *pl);
int listaLlena(const t_list *pl);
int listaVacia(const t_list *pl);
int insertarAlComienzoDeLista(t_list *pl, const t_dato *d);
int eliminarPorClave(t_list *pl, const t_dato *d, int (*comparar)(const t_dato *d1, const t_dato *d2));

#endif // LIST_H_INCLUDED
