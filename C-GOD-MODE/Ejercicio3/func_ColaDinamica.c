#include "colaDinamica.h"
void crearCola(t_cola *cola){
    cola->pri=NULL;
    cola->ult=NULL;
}
int acolar(t_cola *cola,const t_dato *dato){
    t_nodoCD *aux=(t_nodoCD*)malloc(sizeof(t_nodoCD));

    if(aux==NULL)
        return SIN_MEMORIA;

    aux->dato=*dato;
    aux->sig=NULL;

    if(cola->pri==NULL)
        cola->pri=aux;
    else
        cola->ult->sig=aux;

    cola->ult=aux;

    return ACOLADO;

}
int desacolar(t_cola *cola,t_dato *dato){
    t_nodoCD *aux;
    if(cola->pri==NULL)
        return COLA_VACIA;

    *dato=cola->pri->dato;
    aux=cola->pri;
    cola->pri=aux->sig;
    if(cola->pri==cola->ult)
        cola->ult=NULL;

    free(aux);

    return DESACOLADO;
}
void vaciarCola(t_cola *cola){
    t_nodoCD *aux;
    while(cola->pri){
        if(cola->pri==cola->ult)
            cola->ult=NULL;

        aux=cola->pri;
        cola->pri=aux->sig;
        free(aux);

    }
}
int colaVacia(const t_cola *cola){
    if(cola->pri==NULL)
        return VERDADERO;
    return FALSO;
}
int colaLlena(const t_cola *cola){
    t_nodoCD *aux=(t_nodoCD*) malloc(sizeof(t_nodoCD));

    if(aux==NULL)
        return VERDADERO;

    free(aux);
    return FALSO;
}
int verPrimero(const t_cola *cola, t_dato *dato){
    if(cola->pri==NULL)
        return COLA_VACIA;

    *dato=cola->pri->dato;

    return MOSTRAR_DATO;
}