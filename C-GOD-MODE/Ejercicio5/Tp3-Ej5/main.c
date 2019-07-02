// Script Tp3-Ej5

// Trabajo práctico 3
// Ejercicio 5
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
#include <sys/ipc.h>
#include <sys/shm.h>

// Bibliotecas propias.
#include "info.h"
#include "list.h"
#include "functions.h"


int main()
{
    int code;

    FILE *fp;
    t_list lista;
    char *partido = malloc(20);
    char *patente = malloc(8);
    float monto;

    key_t          ShmKEY;
    int            ShmID;
    struct Memory  *ShmPTR;

    crearLista(&lista);
    leerArchivo(&fp, &lista, partido);

    printf("Ingrese el partido de su sede: ");
    scanf("%19[^\n]s", partido);
    normalizarCadena(partido, strlen(partido));

    mostrarMenu();

    while (1) {
        printf("Indique la opcion: ");
        scanf ("%d", &code);
        while (code < 1 || code > 7){
            printf("Opcion invalida, ingrese un numero entre 1 y 7: ");
            scanf ("%d", &code);
        }

        switch (code) {
            case 1:
                printf("Ingrese la patente: ");
                scanf("%7s", patente);
                printf("Ingrese monto de la multa: ");
                scanf("%f", &monto);
                if (ingresarMulta(patente, partido, monto, &lista) == TODO_OK){
                    printf("Se ingreso la multa exitosamente.\n");
                } else{
                    printf("La multa no se pudo generar, intente nuevamente.\n");
                }
                break;
            case 2:
                if (registrosSuspender(&lista , partido) == NOT_OK){
                    printf("No se encontraron registros a suspender.\n");
                }
                break;
            case 3:
                printf("Ingrese la patente: ");
                scanf("%7s", patente);
                if (saldarMulta(patente, partido, &lista) == NOT_OK){
                    printf("La patente ingresada no tiene multas a saldar.\n");
                }
                break;
            case 4:
                printf("Ingrese la patente: ");
                scanf("%7s", patente);
                if(buscarMontoTotal(patente, partido, &lista) == NOT_OK){
                    printf("La patente ingresada no tiene multas pendientes.\n");
                }
                break;
            case 5:
                if (verMontoTotalInfractores (&lista, partido) == NOT_OK){
                    printf("No hay patentes con multas pendientes.\n");
                }
                break;
            case 6:
                mostrarMenu();
                break;
            case 7:
                return TODO_OK;
                break;
            default:
                return NOT_OK;
        }
    }

    return TODO_OK;
}
