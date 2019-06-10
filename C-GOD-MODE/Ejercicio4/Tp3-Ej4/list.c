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

int insertarAlComienzoDeLista(t_list *pl, const t_dato *d){
    // Reservo memoria y pregunto si hay
    t_nodo *nue = (t_nodo *)malloc(sizeof(t_nodo));
    if (!nue){
        return SIN_MEMORIA;
    }
    // Asigno la info correspondiente al nuevo nodo.
    nue -> info = *d;
    // Indico que el nodo siguiente al nuevo es al que apunta actualmente *pl
    nue -> sig = *pl;
    // Le indico a *pl que apunte al nodo nuevo.
    *pl = nue;
    return TODO_OK;
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
