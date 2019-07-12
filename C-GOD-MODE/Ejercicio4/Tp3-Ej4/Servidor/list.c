// Script Tp3-Ej4-Servidor

// Trabajo práctico 3
// Ejercicio 4
// Primera Reentrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869

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
        return (int)SIN_MEMORIA;
    }
    while ( *p ) {
        p = &(*p)->sig;
    }
    nue->info = *d;
    nue->sig = NULL;
    *p = nue;
    return (int)TODO_OK;
}

int eliminarPorClave(t_list *pl, const t_dato *d, t_cmp cmp){
    if(listaVacia(pl) == TODO_OK){
        return (int)LISTA_VACIA;
    }

    while (*pl && cmp(d, &(*pl)->info) != TODO_OK) {
        pl = &(*pl)->sig;
    }
    if (*pl && cmp(d, &(*pl)->info) == TODO_OK) {
        t_nodo *nae;
        nae = *pl;
        *pl = nae->sig;
        free(nae);
        return (int)TODO_OK;
    }
    return (int)NOT_OK;
}

int buscarEnListaNoOrdenadaPorClave (t_list *p, t_dato *d, t_cmp cmp) {
    if(listaVacia(p) == TODO_OK){
        return (int)LISTA_VACIA;
    }

    while ( *p && cmp( &(*p)->info, d ) != TODO_OK ){
        p = &(*p)->sig;
    }

    if(*p) {
        *d = (*p)->info;
        return (int)TODO_OK;
    }

    return (int)NOT_OK;
}

int buscarYActualizar (t_list *p, const t_dato *d, const float monto, t_cmp cmp) {
    
    if(listaVacia(p) == TODO_OK){
        return (int)LISTA_VACIA;
    }

    while ( *p && cmp( &(*p)->info, d ) != TODO_OK ){
        p = &(*p)->sig;
    }

    if(*p) {
	(*p)->info.nombre_titular =  d->nombre_titular;
        (*p)->info.cantidad_multas += 1;
        (*p)->info.monto_total += monto;
        return (int)TODO_OK;
    }

    return (int)NOT_OK;
}

int mostrarLista(t_list *p, t_dato *busqueda, t_cmp cmp, char *response){
     if(listaVacia(p) == TODO_OK){
        return (int)LISTA_VACIA;
    }

    int flag = 0;
    t_dato dato;
    t_list *aux = p;

    char *aux_monto = malloc(15);
    *aux_monto = '\0';

    while(*aux != NULL){
        dato = (*aux)->info;
        if (cmp(&dato, busqueda) == TODO_OK){
                strcat(response, dato.patente);
		if(busqueda->cantidad_multas == 0){
                	strcat(response, " ");
                	sprintf(aux_monto, "%.2f", dato.monto_total);
                	strcat(response, aux_monto);
		}
                strcat(response, "\n");
                flag = 1;
        }
        aux = &(*aux)->sig;
    }
    free(aux_monto);    
    strcat(response, "\0");

    if (flag == 1){
        return (int)TODO_OK;
    } else{
        return (int)NOT_OK;
    }
}

/** FUNCIONES EXTRA */

int compararPatente (const t_dato *d1, const t_dato *d2) {
    if (strcmp(d1->partido, d2->partido) == 0 &&
        strcmp(d1->patente, d2->patente) == 0) {
        return (int)TODO_OK;
    } else {
        return (int)NOT_OK;
    }
}


int compararPartido (const t_dato *d1, const t_dato *d2) {
    if (strcmp(d1->partido, d2->partido) == 0) {
        return (int)TODO_OK;
    } else {
        return (int)NOT_OK;
    }
}

int compararSuspender(const t_dato *d1, const t_dato *d2){
	if(strcmp(d1->partido, d2->partido) == 0){
		if (d1->monto_total > 20000 || d1->cantidad_multas > 3){
			return (int)TODO_OK;
		}
	}
	return (int)NOT_OK;
}



