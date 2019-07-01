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

char *substring(char *string, int position, int length)
{
  char *pointer;
  int c;

  pointer = malloc(length + 1);

  if (pointer == NULL)
  {
    printf("Unable to allocate memory.\n");
    exit(1);
  }

  for (c = 0; c < length; c++)
  {
    *(pointer + c) = *(string + position - 1);
    string++;
  }

  *(pointer + c) = '\0';

  return pointer;
}

int cargoArchivoMaestro(char *fname, unsigned int *ids_array, unsigned int *min_stock_array, unsigned int *real_stock_array)
{
  FILE *fMaster;
  int pos = 0;
  char *str;
  char *id;
  char *minStock;

  str = malloc(MAESTRO_REGISTRY_SIZE + 1);

  fMaster = fopen(fname, "r");

  if (fMaster == NULL){
    char path[1024];
    strcpy(path, ".");
    strcpy(path, fname);
    fMaster = fopen(fname, "r");
    if (fMaster == NULL){
      printf("No se pudo abrir el archivo maestro de articulos.\n");
      return 0;
    }
  }

  while (fgets(str, MAESTRO_REGISTRY_SIZE + 1, fMaster) != NULL)
  {
    id = substring(str, 1, 8);
    minStock = substring(str, 49, 5);

    ids_array[pos] = atoi(id);
    min_stock_array[pos] = atoi(minStock);
    real_stock_array[pos] = 0;
    pos++;
  }

  fclose(fMaster);
  return 1;
}

int cargoArchivoStock(char *fname)
{
  extern t_cola cola;
  t_file *aux = (t_file *)malloc(sizeof(t_file));
  aux->filename = malloc(FILENAME_MAX);

  strcpy(aux->filename, fname);

  if (colaVacia(&cola))
  {
    crearCola(&cola);
  }

  if (acolar(&cola, aux) == 0)
  {
    return 0;
  }
  return 1;
}

int calcularCantRegistros(char *fname)
{
  FILE *fp;
  long int fileSize = 0;

  fp = fopen(fname, "r");
  fseek(fp, 0, SEEK_END);
  fileSize = ftell(fp);
  fclose(fp);

  return fileSize / (MAESTRO_REGISTRY_SIZE + 1);
}

int createOutputFiles(FILE **compras, FILE **stock, FILE **log, char *path)
{
  const char *modo = "w";
  char pathModified[1024]; // or some other number
  strcpy(pathModified, path);
  strcat(pathModified, "Pedido_compras.txt");

  *compras = fopen(pathModified, modo);

  if (!*compras)
  {
    printf("Error al abrir el archivo \"%s\" en el modo \"%s\".\n", "Pedido_compras.txt", modo);
    return 0;
  }

  strcpy(pathModified, path);
  strcat(pathModified, "Stock_consolidado.txt");

  *stock = fopen(pathModified, modo);

  if (!*stock)
  {
    printf("Error al abrir el archivo \"%s\" en el modo \"%s\".\n", "Stock_consolidado.txt", modo);
    return 0;
  }

  strcpy(pathModified, path);
  strcat(pathModified, "process.log");

  *log = fopen(pathModified, modo);

  if (!*log)
  {
    printf("Error al abrir el archivo \"%s\" en el modo \"%s\".\n", "process.log", modo);
    return 0;
  }
  return 1;
}

void* threadRoutine(void *arg)
{

  pthread_mutex_t mutexCola = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t mutexLog1 = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t mutexLog2 = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t mutexStockArray = PTHREAD_MUTEX_INITIALIZER;

  t_thread* thread = (t_thread*)arg;
  extern t_cola cola;
  t_file *sucursalFile = (t_file *)malloc(sizeof(t_file));
  FILE *sucursal;
  char *idStr;
  char *stockStr;
  char *str;
  str = malloc(STOCK_REGISTRY_SIZE + 1);

  while (!colaVacia(&cola))
  {
    pthread_mutex_lock( &mutexCola );
    if(colaVacia(&cola)){
        pthread_mutex_unlock( &mutexCola );
        break;
    }
    desacolar(&cola, sucursalFile);
    pthread_mutex_unlock( &mutexCola );
    sucursal = fopen(sucursalFile->filename, "r");

    if (sucursal)
    {
      pthread_mutex_lock( &mutexLog1 );
      fprintf(thread->log, "THREAD: %d PROCESA ARCHIVO: \"%s\"\n", thread->threadNum, sucursalFile->filename );
      pthread_mutex_unlock( &mutexLog1 );

      while (fgets(str, STOCK_REGISTRY_SIZE + 1, sucursal) != NULL)
      {
        idStr = substring(str, 1, 8);
        stockStr = substring(str, 9, 5);

        int pos = 0;
        int found = 0;
        while (pos < thread->max_position && found == 0)
        {
          if (atoi(idStr) == thread->ids_array[pos])
          {
            found++;
          }
          pos++;
        }

        if (!found)
        {
          pthread_mutex_lock( &mutexLog2 );
          fprintf(thread->log, "ARCHIVO: \"%s\" , THREAD: %d, ARTICULO: %8s NO ENCONTRADO EN MAESTRO\n", sucursalFile->filename, thread->threadNum, idStr);
          pthread_mutex_unlock( &mutexLog2 );

        }
        else
        {
          pthread_mutex_lock( &mutexStockArray );
          thread->real_stock_array[pos] = thread->real_stock_array[pos] + atoi(stockStr);
          pthread_mutex_unlock( &mutexStockArray );

        }
      }
      fclose(sucursal);
    }
  }
  return 0;
}

int procesarArchivos(int cantThreads, char *path, unsigned int *ids_array, unsigned int *min_stock_array, unsigned int *real_stock_array, int max_position)
{
  extern t_cola cola;

  FILE *compras;
  FILE *stock;
  FILE *log;
  char *idStr;
  char *stockStr;
  char *stockCompraStr;

  t_thread *threads;

  stockCompraStr = malloc(5 + 1);
  idStr = malloc(8 + 1);
  stockStr = malloc(5 + 1);

  createOutputFiles(&compras, &stock, &log, path);

  if (colaVacia(&cola))
  {
    printf("No hay archivos de sucursales a procesar. \n");
    fprintf(log, "No hay archivos de sucursales a procesar. \n");
  }

  threads = (t_thread *)calloc(cantThreads, sizeof(t_thread));
  if (threads == NULL)
  {
    printf("Error al reservar memoria para la estructura de threads.\n");
    fprintf(log, "Error al reservar memoria para la estructura de threads.\n");
    return 1;
  }

  for (int i = 0; i < cantThreads; i++)
  {
    threads[i].threadNum = i + 1;
    threads[i].max_position = max_position;
    threads[i].ids_array = ids_array;
    threads[i].real_stock_array = real_stock_array;
    threads[i].log = log;

    if (pthread_create(&threads[i].tid, NULL, &threadRoutine, &threads[i]))
    {
      printf("Error al crear el thread nro: %d\n", threads[i].threadNum);
      fprintf(log, "Error al crear el thread nro: %d\n", threads[i].threadNum);
      return 1;
    }
  }

  for (int i = 0; i < cantThreads; i++)
  {
      if ( pthread_join(threads[i].tid, NULL) )
      {
          printf("Error al esperar finalización de thread nro: %d\n", threads[i].threadNum);
          fprintf(log, "Error al esperar finalización de thread nro: %d\n", threads[i].threadNum);
          return 1;
      }
  }

  vaciarCola(&cola);

  int pos = 0;
  while (pos < max_position)
  {
    sprintf(idStr, "%d", ids_array[pos]);
    if(real_stock_array[pos] > 99999){
        fprintf(log,  "ARTICULO: %08d, EXCEDE LIMITE. STOCK REAL: %d\n", ids_array[pos] , real_stock_array[pos]);
        strcpy(stockStr, "99999");
    }else{
       sprintf(stockStr, "%05d", real_stock_array[pos]);
    }

    if (min_stock_array[pos] > real_stock_array[pos])
    {
      sprintf(stockCompraStr, "%05d", min_stock_array[pos] + min_stock_array[pos] - real_stock_array[pos]);
      fprintf(compras, "%08d%5s\n", ids_array[pos], stockCompraStr );
    }
    fprintf(stock, "%08d%5s\n", ids_array[pos], stockStr);
    pos++;
  }

  fclose(compras);
  fclose(stock);
  fclose(log);

  fprintf(log, "Proceso finalizado.\n");
  printf("Proceso finalizado.\n");

  return 1;
}

void mostrarArray(unsigned int *array, int max_position)
{
  int pos = 0;
  while (pos < max_position)
  {
    printf("%d\n", array[pos]);
    pos++;
  }
}
