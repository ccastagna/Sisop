#include "lista.h"

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

int eliminarPorClave(t_list *pl, const t_dato *d, int (*comparar)(const t_dato *d1, const t_dato *d2)){
    while (*pl && comparar(d, &(*pl)->info) != 0) {
        pl = &(*pl)->sig;
    }
    if (*pl && comparar(d, &(*pl)->info == 0)) {
        t_nodo *nae;
        nae = *pl;
        *pl = nae->sig;
        free(nae);
        return VERDADERO;
    }
    return FALSO;
}

int buscarEnListaNoOrdenadaPorClave (t_list *p, t_dato *d, int a) {
    t_nodo *aux;
    while ( *p && comparar( &(*p)->info, d ) != 0 ){
        p = &(*p)->sig;
    }

    if(*p) {
        aux = *p;
        *d = aux->info;
        if( ELIMINAR_O_NO == ELIMINAR_NODO ) {
            *p = aux->sig;
            free(aux);
        }
        return 1;
    }
    return 0;
}
