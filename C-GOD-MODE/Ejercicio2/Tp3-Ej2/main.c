// Script Tp3-Ej2

// Trabajo practico 3
// Ejercicio 2
// Entrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869

#include "functions.h"
#include "colaDinamica.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

#define MAX_THREAD_COUNT 15488 // kernel.threads-max = 15488

int main(int argc, char *argv[])
{
  char *pathSeparator = "/";
  DIR *dir;
  struct dirent *ent;
  long int contArticulos = 0;
  char pathSucursal[1024];
  char path[1024];
  unsigned int *ids_array;
  unsigned int *min_stock_array;
  unsigned int *real_stock_array;


  if (!strcmp(argv[1], "-h"))
  {
    printf("Proceso para calcular el stock consolidado y generar archivo de compras.\nEl proceso toma por parametro la cantidad de threads de procesamiento, el directorio \n");
    printf("donde se encuentran los archivos de las sucursales y el archivo del maestro de productos; \ny genera como salida el archivo de stock consolidado (Stock_consolidado.txt) y \n");
    printf("el archivo de pedido de compras (Pedido_compras.txt), mas un archivo de log (process.log) \nque indica informacion de la ejecucion.\n");
    printf("\nPrimer parametro:\tcantidad de hilos a utilizar durante el procesamiento.\n");
    printf("Segundo parametro:\tpath del directorio donde se encuentran los archivos de stock.\n");
    printf("Tercer parametro:\tpath del archivo de articulos.\n\n");
    printf("Por ejemplo:\n\n\t./Tp3-Ej2 7 ../TP3_Ej2 ../TP3_Ej2/articulos.txt\n\t./Tp3-Ej2 5 /home/files /home/files/articulos.txt\n\t./Tp3-Ej2 1 ../files/process ../files/articulos.txt\n\t./Tp3-Ej2 10 /home/files/stock /home/stock/articulos.txt\n\nEl directorio pasado por parametro debe contener todos los archivos de stock de sucursales.\nEl archivo maestro de articulos indica para cada uno de los articulos el stock minimo que debe\nhaber en la empresa. \nSi el stock esta por debajo de este numero, el proceso debe generar un registro \nde pedido de compra informando la cantidad necesaria a comprar para que el stock \nquede al doble del stock minimo del articulo.\nEj, Aceite, stock minimo 100, stock actual 73, entonces genera un pedido por 127.\nEl formato de los archivos de stock es de 8 posiciones para el id del articulo y \n5 posiciones para el stock.\nEn caso de que el stock real supere 99999, el mismo se informara en el log con su stock real, \npero en el archivo de stock consolidado quedara como 99999. \nTambien se informara en el archivo de log si algun articulo no se encuentra en el maestro de articulos.\n");
    return 0;
  }

  if (argc != 4)
  {
    printf("La aplicacion espera 3 parametros la cantidad de threads a crear, \nel directorio donde se encuentran los archivos a procesar y el archivo maestro de articulos. \n");
    printf("Por ejemplo: ./Tp3-Ej2 5 ../files ../files/articulos.txt\n");
    printf("Para mas informacion, ejecute de la siguiente manera: ./-h \n");
    return 0;
  }

  if (!isNumber(argv[1]) || atoi(argv[1]) <= 0 || atoi(argv[1]) > MAX_THREAD_COUNT)
  {
    printf("El primer parametro debe ser un numero (cantidad de threads a crear). \n");
    return 0;
  }

  dir = opendir(argv[2]);
  if (dir == NULL)
  {
    printf("El segundo parametro debe ser un path valido. \n");
    return 0;
  }

  strcpy(path, argv[3]);
  contArticulos = calcularCantRegistros(path);

  ids_array = (unsigned int *)malloc(sizeof(int) * contArticulos);
  min_stock_array = (unsigned int *)malloc(sizeof(int) * contArticulos);
  real_stock_array = (unsigned int *)malloc(sizeof(int) * contArticulos);

  cargoArchivoMaestro(path, ids_array, min_stock_array, real_stock_array);


  while ((ent = readdir(dir)) != NULL)
  {
    strcpy(pathSucursal, argv[2]);
    strcat(pathSucursal, pathSeparator);
    strcat(pathSucursal, ent->d_name);

    if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") || !strcmp(ent->d_name, "Stock_consolidado.txt") || !strcmp(ent->d_name, "process.log") || !strcmp(ent->d_name, "Pedido_compras.txt") || !strcmp(ent->d_name, "articulos.txt"))
    {
      // do nothing (straight logic)
    }
    else
    {
      strcat(pathSucursal, "\0");
      cargoArchivoStock(pathSucursal);

    }
  }

  closedir(dir);

  strcpy(path, argv[2]);
  strcat(path, pathSeparator);

  printf("Comienza el procesamiento\n");

  procesarArchivos(atoi(argv[1]), path, ids_array, min_stock_array, real_stock_array, contArticulos);

  return 1;
}
