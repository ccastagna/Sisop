// Script Tp3-Ej5-Client

// Trabajo pr�ctico 3
// Ejercicio 5
// ReEntrega

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
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

// Bibliotecas propias.
#include "functions.h"


int main()
{
    int code;

    //t_list lista;
    char *partido = malloc(20);
    char *patente = malloc(8);
    char *nombre_titular = malloc(25);
    float monto;

    key_t          ShmKEY;
    int            ShmID;
    struct t_buffer  *buffer;
    
    ShmKEY = ftok("../Server/", 'x');
    ShmID = shmget(ShmKEY, sizeof(t_buffer), IPC_EXCL | 0666);
    if (ShmID < 0) {
        printf("*** shmget error (client) ***\n");
        exit(1);
    }
    printf("   Client has received a shared memory of four integers...\n");
    
    sem_t *clientSem = sem_open("Client",O_EXCL);
    sem_t *requestSem = sem_open("Request",O_EXCL);
    sem_t *responseSem = sem_open("Response",O_EXCL);


    buffer = (struct t_buffer *) shmat(ShmID, NULL, 0);
    printf("buffer by shmat %p \n", buffer);  

    while (1) {
        sem_wait(clientSem);

        limpiarBuffer(buffer);

        printf("Ingrese el partido de su sede: ");
        scanf("%19[^\n]s", buffer->multas[0].partido);
        normalizarCadena(buffer->multas[0].partido, strlen(buffer->multas[0].partido));

        mostrarMenu();

        printf("Indique la opcion: ");
        scanf ("%d", &code);
        while (code < 1 || code > 7){
            printf("Opcion invalida, ingrese un numero entre 1 y 7: ");
            scanf ("%d", &code);
        }

        buffer->opcion = code;

        switch (code) {
            case 1:

                printf("Ingrese la patente: ");
                scanf("%7s", buffer->multas[0].patente);
                printf("Ingrese monto de la multa: ");
                scanf("%f", buffer->multas[0].monto_total);
                printf("Ingrese el nombre del titular: ");
                fflush(stdin);
                scanf("%24[^\n]s", buffer->multas[0].nombre_titular);
                fflush(stdin);

                sem_post(requestSem);
                sem_wait(responseSem);

                printf("%s",buffer->msg);
               
                sem_post(clientSem);
                break;
            case 2:

                sem_post(requestSem);
                sem_wait(responseSem);

                for(int i=0; i <= buffer->cantMultas ; i++ ){
                    printf("%s\n", *(buffer->multas[i].patente));
                }

                printf("%s",buffer->msg);

                sem_post(clientSem);
                break;
            case 3:

                printf("Ingrese la patente: ");
                scanf("%7s", buffer->multas[0].patente);
                
                sem_post(requestSem);
                sem_wait(responseSem);

                printf("%s",buffer->msg);
               
                sem_post(clientSem);
                break;
            case 4:

                printf("Ingrese la patente: ");
                scanf("%7s", buffer->multas[0].patente);
                
                sem_post(requestSem);
                sem_wait(responseSem);

                printf("%s\t%.2f\n", buffer->multas[0].patente, buffer->multas[0].monto_total);
                printf("%s",buffer->msg);
               
                sem_post(clientSem);
                break;
            case 5:
                
                sem_post(requestSem);
                sem_wait(responseSem);

                for(int i=0; i <= buffer->cantMultas ; i++ ){
                    printf("%s\t%.2f\n", buffer->multas[0].patente, buffer->multas[0].monto_total);
                }
                printf("%s",buffer->msg);

                sem_post(clientSem);
                break;
            case 6:
                mostrarMenu();
                break;
            case 7:
                shmdt((void *) buffer);
                sem_post(clientSem);
                exit(0);
                break;
            default:
                mostrarMenu();
                break;
        }
    }

    return TODO_OK;
}
