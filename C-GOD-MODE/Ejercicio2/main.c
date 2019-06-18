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

int main()
{
    int code;

    FILE *fp;
    t_list lista;
    char *partido = "Lanus";
    char *patente = malloc (sizeof(7));
    float monto;

    crearLista(&lista);
    leerArchivo(&fp, &lista, partido);

    printf("Menu de Opciones: \n");
    printf("1. Ingresar multa.\n");
    printf("2. Mostrar registros a suspender.\n");
    printf("3. Saldar multa.\n");
    printf("4. Buscar monto total de un infractor.\n");
    printf("5. Buscar monto total a pagar de todos los infractores.\n");
    printf("6. Salir.\n");

    while (1) {
        printf("\n\nIndique la opcion: ");
        scanf ("%d", &code);
        while (code < 1 || code > 6){
            printf("\nOpcion invalida, ingrese una entre 1 y 6.");
            scanf ("%d", &code);
        }

        switch (code) {
            case 1:
                printf("\nIngrese la patente: ");
                scanf("%s", patente);
                printf("\nIngrese monto de la multa: ");
                scanf("%f", &monto);
                ingresarMulta(patente, partido, monto, &lista);
                break;
            case 2:
                registrosSuspender(&lista);
                break;
            case 3:
                printf("\nIngrese la patente: ");
                scanf("%s", patente);
                saldarMulta(patente, partido, &lista);
                break;
            case 4:
                break;
            case 5:
                verMontoTotalInfractores (&lista, partido);
                break;
            case 6:
                return 1;
                break;
            default:
                return 0;
        }
    }

    return 1;
}
