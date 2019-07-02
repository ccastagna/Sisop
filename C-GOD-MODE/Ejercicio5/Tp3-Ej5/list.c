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
        return SIN_MEMORIA;
    }
    while ( *p ) {
        p = &(*p)->sig;
    }
    nue->info = *d;
    nue->sig = NULL;
    *p = nue;
    return TODO_OK;
}

int eliminarPorClave(t_list *pl, const t_dato *d, t_cmp cmp){
    if(listaVacia(pl) == TODO_OK){
        return LISTA_VACIA;
    }

    while (*pl && cmp(d, &(*pl)->info) != TODO_OK) {
        pl = &(*pl)->sig;
    }
    if (*pl && cmp(d, &(*pl)->info) == TODO_OK) {
        t_nodo *nae;
        nae = *pl;
        *pl = nae->sig;
        free(nae);
        return TODO_OK;
    }
    return NOT_OK;
}

int buscarEnListaNoOrdenadaPorClave (t_list *p, t_dato *d, t_cmp cmp) {
    if(listaVacia(p) == TODO_OK){
        return LISTA_VACIA;
    }

    while ( *p && cmp( &(*p)->info, d ) != TODO_OK ){
        p = &(*p)->sig;
    }

    if(*p) {
        *d = (*p)->info;
        return TODO_OK;
    }

    return NOT_OK;
}

int buscarYActualizar (t_list *p, const t_dato *d, const float monto, t_cmp cmp) {
    if(listaVacia(p) == TODO_OK){
        return LISTA_VACIA;
    }

    while ( *p && cmp( &(*p)->info, d ) != TODO_OK ){
        p = &(*p)->sig;
    }

    if(*p) {
        (*p)->info.cantidad_multas += 1;
        (*p)->info.monto_total += monto;
        return TODO_OK;
    }

    return NOT_OK;
}

int mostrarLista(t_list *p, const char *partido, t_cmp cmp){
     if(listaVacia(p) == TODO_OK){
        return LISTA_VACIA;
    }

    int flag = 0;
    t_dato *d = (t_dato *) malloc (sizeof (t_dato *));
    t_list *aux = p;

    while (*aux){
        *d = (*aux)->info;
        if (cmp( d, partido) == TODO_OK) {
            flag = 1;
            printf("%s\t%.2f\n", d->patente, d->monto_total);
            fflush(stdin);
        }
        aux = &(*aux)->sig;
    }

    if (flag == 1){
        return TODO_OK;
    } else {
        return NOT_OK;
    }
}

/** FUNCIONES EXTRA */

int *compararPatente (const t_dato *d1, const t_dato *d2) {
    if (strcmp(d1->partido, d2->partido) == 0 &&
        strcmp(d1->patente, d2->patente) == 0) {
        return TODO_OK;
    } else {
        return NOT_OK;
    }
}


int *compararPartido (const t_dato *d1, const char *d2) {
    if (strcmp(d1->partido, d2) == 0) {
        return TODO_OK;
    } else {
        return NOT_OK;
    }
}
