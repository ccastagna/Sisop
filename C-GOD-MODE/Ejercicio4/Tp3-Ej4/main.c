// Script Tp3-Ej4

// Trabajo práctico 3
// Ejercicio 4
// Entrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869

// Bibliotecas pre-compiladas.
#include <stdio.h>
#include <stdlib.h>

// Bibliotecas propias.
#include "info.h"
#include "list.h"
#include "functions.h"


int main()
{
    int code = 0;
    t_list lista;
    char *partido = "Lanus";
    FILE *fp;
    crearLista(&lista);
    leerArchivo(&fp, &lista, partido);

    printf("Menu de Opciones: \n");
    printf("1. Ingresar multa.\n");
    printf("2. Mostrar registros a suspender.\n");
    printf("3. Saldar multa.\n");
    printf("4. Buscar monto total de un infractor.\n");
    printf("5. Buscar monto total a pagar de todos los infractores.\n");
    printf("6. Salir.\n");

    while (code != 6) {
        printf("\n\nIndique la opcion: ");
        scanf ("%d", &code);
        while (code < 1 || code > 6){
            printf("\nOpcion invalida, ingrese una entre 1 y 6.");
            scanf ("%d", &code);
        }

        switch (code) {
            case 1:
                break;
            case 2:
                break;
            case 3:
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
