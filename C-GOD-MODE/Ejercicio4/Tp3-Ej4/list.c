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
        return NOT_OK;
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

int buscarEnListaNoOrdenadaPorClave (t_list *p, t_dato *d, char* a, t_cmp cmp) {
    while ( *p && cmp( &(*p)->info, d ) != 0 ){
        p = &(*p)->sig;
    }

    if(*p) {
        *d = (*p)->info;
        return TODO_OK;
    }

    return NOT_OK;
}

int mostrarLista(t_list *p, char *partido, t_cmp cmp){
    int flag = 0;
    t_dato *d = (t_dato *) malloc (sizeof (t_dato *));
    while (*p){
        *d = (*p)->info;
        if (cmp( d, partido) == TODO_OK) {
            flag = 1;
            printf("%s\t%.2f\n", d->patente, d->monto_total);
        }
        p = &(*p)->sig;
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
