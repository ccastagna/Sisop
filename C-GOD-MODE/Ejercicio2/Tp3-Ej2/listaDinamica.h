#ifndef LISTADINAMICA_H_INCLUDED
#define LISTADINAMICA_H_INCLUDED

#include "info.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct s_nodoMaestro{
    struct s_nodoMaestro *sig;
    t_dato dato;
    long stockTotal;

}t_nodoMaestro;

typedef struct{
    t_nodoMaestro *pri;
    t_nodoMaestro *ult;

}t_lista;

void crearLista(t_lista *lista);
int ponerEnLista(t_lista *lista,const t_dato *dato);
void vaciarLista(t_lista *lista);
int listaVacia(const t_lista *lista);
int listaLlena(const t_lista *lista);
int buscarEnListaPorId(const t_lista *lista, t_dato *dato, char * id);
int mostrarLista(t_lista *p);


#endif // LISTADINAMICA_H_INCLUDED

