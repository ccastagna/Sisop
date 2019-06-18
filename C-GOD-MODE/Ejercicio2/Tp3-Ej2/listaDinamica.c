#include "listaDinamica.h"
void crearLista(t_lista *lista){
    lista->pri=NULL;
    lista->ult=NULL;
}
int ponerEnLista(t_lista *lista,const t_dato *dato){
    t_nodoMaestro *aux=(t_nodoMaestro*)malloc(sizeof(t_nodoMaestro));

    if(aux==NULL)
        return 0;

    aux->dato=*dato;
    aux->sig=NULL;

    if(lista->pri==NULL)
        lista->pri=aux;
    else
        lista->ult->sig=aux;

    lista->ult=aux;

    return 1;

}
void vaciarLista(t_lista *lista){
    t_nodoMaestro *aux;
    while(lista->pri){
        if(lista->pri==lista->ult)
            lista->ult=NULL;

        aux=lista->pri;
        lista->pri=aux->sig;
        free(aux);

    }
}
int listaVacia(const t_lista *lista){
    if(lista->pri==NULL)
        return 1;
    return 0;
}
int listaLlena(const t_lista *lista){
    t_nodoMaestro *aux=(t_nodoMaestro*) malloc(sizeof(t_nodoMaestro));

    if(aux==NULL)
        return 1;

    free(aux);
    return 0;
}

int buscarEnListaPorId(const t_lista *p, t_dato *dato, char * id){
    t_dato *masterNode=(t_dato*)malloc(sizeof(t_dato));
    t_nodoMaestro *aux;

    aux = p->pri;
    *masterNode=aux->dato;

    if(strcmp(masterNode->dato.id, id, 8)== 0){
        return 1;
    }

    while (aux->sig){
        aux = aux->sig;
        *masterNode=aux->dato;

        if(strcmp(masterNode->dato.id, id, 8)== 0){
            printf("Encontro");
            return 1;
        }
    }
    return 0;

}

int mostrarLista(t_lista *p){
    t_dato *masterNode=(t_dato*)malloc(sizeof(t_dato));
    t_nodoMaestro *aux;

    aux = p->pri;
    *masterNode=aux->dato;
//    printf("%s\t%40s%5s\n", masterNode->dato.id, masterNode->dato.description, masterNode->dato.stockMinimo);
        printf("%s\n", masterNode->dato.id);

    while (aux->sig){
        aux = aux->sig;
        *masterNode=aux->dato;
//        printf("%s\t%40s%5s\n", masterNode->dato.id, masterNode->dato.description, masterNode->dato.stockMinimo);
        printf("%s\n", masterNode->dato.id);
    }

    printf("LISTO");
    return 1;
}
