#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <functions.h>

#define FALSO -1
#define TODO_OK 0
#define SIN_MEMORIA 1
#define DUPLICADO 2
#define LISTA_VACIA 3

#define ELIMINAR_O_NO   0
#define ELIMINAR_NODO   1

typedef struct s_nodo{
    t_dato info;
    struct s_nodo *sig;
}t_nodo;


typedef t_nodo *t_list;
//typedef int (*t_cmp) (const void *, const void *);

void crearLista(t_list *);
int listaLlena(const t_list *);
int listaVacia(const t_list *);
int insertarAlFinal(t_list *, const t_dato * );
int eliminarPorClave(t_list l, const t_dato *, int (*comparar)(const t_dato *, const t_dato *));
int buscarEnListaNoOrdenadaPorClave (t_list *, t_dato *, int );

#endif // LIST_H_INCLUDED
