// Script Tp3-Ej5-Server

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
#include <signal.h>

// Bibliotecas propias.
#include "info.h"
#include "list.h"
#include "functions.h"

struct t_buffer  *buffer;
sem_t *clientSem, *requestSem, *responseSem;

void cerrarServer(int signum){
    printf("terminando server...");
    shmdt((void *) buffer);
    printf("Server has detached its shared memory...\n");
    shmctl(buffer, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");
    sem_destroy(clientSem);
    sem_destroy(requestSem);
    sem_destroy(responseSem);
    exit(0);
}

int main()
{
    int code;

    FILE *fp;
    t_list lista;

    key_t          ShmKEY;
    int            ShmID;

    ShmKEY = ftok(".", 'x');
    ShmID = shmget(ShmKEY, sizeof(t_buffer), IPC_CREAT | 0666);
    if (ShmID < 0) {
        printf("*** shmget error (server) ***\n");
        exit(1);
    }
    printf("Server has received a shared memory of four integers...\n");
    
    buffer = (t_buffer *) shmat(ShmID, NULL, 0);
    
    printf("ShmPTR by shmat %p \n", buffer);  
    
    printf("Server has attached the shared memory...\n");
    
    clientSem = sem_open("Client",O_CREAT, O_RDWR, 1);
    requestSem = sem_open("Request",O_CREAT, O_RDWR, 0);
    responseSem = sem_open("Response",O_CREAT, O_RDWR, 0);

    crearLista(&lista);
    leerArchivo(&fp, &lista);

    signal(SIGQUIT, cerrarServer);

    while (1) {
        sem_wait(requestSem);

        switch (buffer->opcion) {
            case 1:
                if (ingresarMulta(buffer->multas[0].patente, buffer->multas[0].partido, buffer->multas[0].monto, buffer->multas[0].nombre_titular, &lista) == TODO_OK){
                    printf("Se ingreso la multa exitosamente.\n");
                    buffer->msg = "Se ingreso la multa exitosamente.\n";
                } else{
                    printf("La multa no se pudo generar, intente nuevamente.\n");
                    buffer->msg = "La multa no se pudo generar, intente nuevamente.\n";
                }

                sem_post(responseSem);
                break;
            case 2:
                if (registrosSuspender(buffer, &lista , buffer->multas[0].partido) == NOT_OK){
                    printf("No se encontraron registros a suspender.\n");
                    buffer->msg = "No se encontraron registros a suspender.\n";
                }
                break;
            case 3:
                if (saldarMulta(buffer->multas[0].patente, buffer->multas[0].partido, &lista) == NOT_OK){
                    printf("La patente ingresada no tiene multas a saldar.\n");
                    buffer->msg = "La patente ingresada no tiene multas a saldar.\n";
                }

                sem_post(responseSem);
                break;
            case 4:
                if(buscarMontoTotal(buffer, buffer->multas[0].patente, buffer->multas[0].partido, &lista) == NOT_OK){
                    printf("La patente ingresada no tiene multas pendientes.\n");
                    buffer->msg = "La patente ingresada no tiene multas pendientes.\n";
                }

                sem_post(responseSem);
                break;
            case 5:
                if (verMontoTotalInfractores (buffer, &lista, buffer->multas[0].partido) == NOT_OK){
                    printf("No hay patentes con multas pendientes.\n");
                    buffer->msg = "No hay patentes con multas pendientes.\n";
                }

                sem_post(responseSem);
                break;
            default:
                return NOT_OK;
        }
    }

    return TODO_OK;
}
