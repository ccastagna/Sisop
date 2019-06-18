// Script Tp3-Ej2

// Trabajo pr�ctico 3
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

#define pathSeparator '//'


int main(int argc, char *argv[])
{
    DIR *dir;

    struct dirent *ent;

    if (argc != 3)
    {
        printf("La aplicacion espera como parametros la cantidad de threads a crear \n y el directorio donde se encuentran los archivos a procesar. \n");
        printf("Por ejemplo: ./5 /home/files. \n");
        return 0;
    }

    if(!isNumber(argv[1])){
        printf("El primer parametro debe ser un numero (cantidad de threads a crear). \n");
        return 0;
    }
    /* Empezaremos a leer en el directorio actual */
    dir = opendir(argv[2]);

    /* Miramos que no haya error */
    if (dir == NULL){
        printf("El segundo parametro debe ser un path valido. \n");
        return 0;
    }

    while ((ent = readdir(dir)) != NULL)
    {
        char path[1024] ;   // or some other number
        strcpy( path, argv[2] );
        strcat( path, "\\" ) ; // or "\\" in Windows
        strcat( path, ent->d_name);

        if ( !strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") || !strcmp(ent->d_name, "Stock_consolidado.txt") || !strcmp(ent->d_name, "process.log") || !strcmp(ent->d_name, "Pedido_compras.txt"))
        {
     // do nothing (straight logic)
        }else if ((strcmp(ent->d_name, "articulos.txt") == 0))
        {
            cargoArchivoMaestro(path);
        }
        else
        {
            strcat(path, "\0");
            cargoArchivoStock(path);
        }
    }

    closedir(dir);

    char path[1024] ;   // or some other number
    strcpy( path, argv[2] );
    strcat( path, "\\" ) ; // or "\\" in Windows

    procesarArchivos(atoi(argv[1]), path );

    return 1;
}
