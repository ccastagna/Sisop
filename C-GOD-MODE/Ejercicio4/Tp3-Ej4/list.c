#include "list.h"

/** PRIMITIVAS */

void crearLista(t_list *pl){
    *pl = NULL;
}

int listaLlena(const t_list *pl){
    void *pv = malloc(sizeof(t_nodo));
    free(pv);
    return pv == NULL;
}

int listaVacia(const t_list *pl){
    return *pl == NULL;
}

int insertarAlFinal(t_list *p, const t_dato *d) {
    t_nodo *nue = ( t_nodo *) malloc( sizeof(t_nodo) );
    if ( nue == NULL ) {
        return 0;
    }
    while ( *p ) {
        p = &(*p)->sig;
    }
    nue->info = *d;
    nue->sig = NULL;
    *p = nue;
    return 1;
}

int eliminarPorClave(t_list *pl, const t_dato *d, t_cmp cmp){
    while (*pl && cmp(d, &(*pl)->info) != 0) {
        pl = &(*pl)->sig;
    }
    if (*pl && cmp(d, &(*pl)->info) == 0) {
        t_nodo *nae;
        nae = *pl;
        *pl = nae->sig;
        free(nae);
        return TODO_OK;
    }
    return NOT_OK;
}

int buscarEnListaNoOrdenadaPorClave (t_list *p, t_dato *d, int a, t_cmp cmp) {
    t_nodo *aux;
    while ( *p && cmp( &(*p)->info, d ) != 0 ){
        p = &(*p)->sig;
    }

    if(*p) {
        aux = *p;
        *d = aux->info;
        return TODO_OK;
    }

    return NOT_OK;
}

/** FUNCIONES EXTRA */

int *compararPatente (const void *d1, const void *d2) {
    t_dato *pd1 = (t_nodo *)d1;
    t_dato *pd2 = (t_nodo *)d2;
    if (strcmp(pd1->partido, pd2->partido) == 0 && strcmp(pd1->patente, pd2->patente) == 0) {
        return TODO_OK;
    } else {
        return NOT_OK;
    }
}
