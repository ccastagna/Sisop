#include "functions.h"
#include "listaDinamica.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isNumber(char number[])
{
    int i = 0;

    //checking for negative numbers
    if (number[0] == '-')
        i = 1;
    for (; number[i] != 0; i++)
    {
        if (number[i] > '9' || number[i] < '0')
            return 0;
    }
    return 1;
}

int cargoArchivoMaestro(char * fname){
  extern t_lista lista;

  FILE *fMaster;
  long read = 0;


  t_maestro *registry=(t_maestro*)malloc(sizeof(t_maestro));
  t_dato *list_node=(t_dato*)malloc(sizeof(t_dato));

  crearLista(&lista);
  fMaster=fopen(fname, "r");

  read = fread(registry, sizeof(t_dato), 1, fMaster);

  while (read > 0){
    list_node->dato = *registry;
    list_node->stockActual = 0;
    if(ponerEnLista(&lista,list_node) == 0){
        return 0;
    }
    read = fread(registry, sizeof(t_dato), 1, fMaster);
  }
  fclose(fMaster);
  return 1;
}

int cargoArchivoStock(char * fname){
  extern t_cola cola;
  t_file *aux=(t_file*)malloc(sizeof(t_file));
  aux->filename = malloc( FILENAME_MAX );

  strcpy(aux->filename, fname);

  if(colaVacia(&cola)){
      crearCola(&cola);
  }

  if(acolar(&cola,aux) == 0){
    return 0;
  }
  return 1;
}

int createOutputFiles(FILE **compras,FILE ** stock, FILE **log, char * path){
    const char * modo = "w";
    char pathModified[1024] ;   // or some other number
    strcpy( pathModified, path );
    strcat( pathModified, "Pedido_compras.txt" );

    *compras = fopen(pathModified, modo);

    if(!*compras){
       printf("Error al abrir el archivo \"%s\" en el modo \"%s\".\n","Pedido_compras.txt",modo);
       return 0;
    }

    strcpy( pathModified, path );
    strcat( pathModified, "Stock_consolidado.txt" );

    *stock = fopen(pathModified, modo);

    if(!*stock){
        printf("Error al abrir el archivo \"%s\" en el modo \"%s\".\n","Stock_consolidado.txt",modo);
        return 0;
    }

    strcpy( pathModified, path );
    strcat( pathModified, "process.log" );

    *log = fopen(pathModified, modo);

    if(!*log){
        printf("Error al abrir el archivo \"%s\" en el modo \"%s\".\n","process.log",modo);
        return 0;
    }
    return 1;

}

int procesarArchivos(int cantThreads, char * path){
    extern t_cola cola;
    extern t_lista lista;
    t_file *sucursalFile=(t_file*)malloc(sizeof(t_file));
    t_stock *stockRegistry=(t_stock*)malloc(sizeof(t_stock));
    t_dato *masterNode=(t_dato*)malloc(sizeof(t_dato));
    t_nodoMaestro *aux;

    long stockMinimo = 0;
    FILE *compras;
    FILE *stock;
    FILE *log;
    FILE *sucursal;


    createOutputFiles(&compras, &stock, &log, path);

    if(colaVacia(&cola)){
        printf("No hay archivos de sucursales a procesar. \n");
    }

    mostrarLista(&lista);
    return 0;
    while(!colaVacia(&cola)){
        desacolar( &cola, sucursalFile);
        sucursal = fopen(sucursalFile->filename, "r");


        if(sucursal){
            fprintf(log, "Se esta procesando el archivo %s \n", sucursalFile->filename);
            while(fread(stockRegistry, sizeof(t_stock), 1, sucursal)){
                if(!buscarEnListaPorId(&lista, masterNode, stockRegistry->id)){
                    fprintf(log, "El articulo %8s no se encontro en el maestro de articulos \n", stockRegistry->id);
                }
                else{
                    masterNode->stockActual = masterNode->stockActual + atol(stockRegistry->stock);
                }

            }
            fclose(sucursal);
        }
    }


    fprintf(compras, "ID      CANTIDAD A PEDIR\n");
    fprintf(stock, "ID      DESCRIPCION                             STOCK\n");
    aux = lista.pri;
    *masterNode=aux->dato;
    stockMinimo = atol(masterNode->dato.stockMinimo);
    if( stockMinimo < masterNode->stockActual){
        fprintf(compras, "%8s%05ld\n", masterNode->dato.id, stockMinimo + stockMinimo - masterNode->stockActual);
    }
    fprintf(stock, "%8s%40s%05ld\n", masterNode->dato.id, masterNode->dato.description, stockMinimo + stockMinimo - masterNode->stockActual);

    while(aux->sig != NULL){
        aux = aux->sig;
        *masterNode=aux->dato;
        stockMinimo = atol(masterNode->dato.stockMinimo);
        if( stockMinimo < masterNode->stockActual){
            fprintf(compras, "%8s%05ld\n", masterNode->dato.id, stockMinimo + stockMinimo - masterNode->stockActual);
        }
        fprintf(stock, "%8s%40s%05ld\n", masterNode->dato.id, masterNode->dato.description, stockMinimo + stockMinimo - masterNode->stockActual);
    }
    return 1;

    fclose(compras);
    fclose(stock);
    fclose(log);
    return 1;
}

