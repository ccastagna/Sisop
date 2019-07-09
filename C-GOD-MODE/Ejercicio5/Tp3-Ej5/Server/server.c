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
#include <signal.h>

// Bibliotecas propias.
#include "info.h"
#include "list.h"
#include "functions.h"

t_buffer  *buffer;
int clientSem, requestSem, responseSem, ShmID;

void cerrarServer(int signum){
    printf("terminando server...\n");
    shmdt((void *) buffer);
    printf("Server has detached its shared memory...\n");
    shmctl(ShmID, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");
    eliminarSemaforo(clientSem);
    eliminarSemaforo(requestSem);
    eliminarSemaforo(responseSem);
    exit(0);
}

int main()
{
    int code;

    FILE *fp;
    t_list lista;

    key_t          ShmKEY;

    ShmKEY = ftok(".", 'x');
    ShmID = shmget(ShmKEY, sizeof(t_buffer), IPC_CREAT | 0666);
    if (ShmID < 0) {
        printf("*** shmget error (server) ***\n");
        exit(1);
    }
    printf("Server has received a shared memory of four integers...\n");
    
    buffer = (t_buffer *) shmat(ShmID, NULL, 0);
    
    printf("buffer by shmat %p \n", buffer);  
    
    printf("Server has attached the shared memory...\n");

    key_t clientSemTok = ftok(".",1000);
    key_t requestSemTok = ftok(".",1001);
    key_t responseSemTok = ftok(".",1002);

    clientSem = obtenerSemaforo(clientSemTok,1);
    printf("\nIdentificador del semaforo de cliente: %d\n",clientSem);
    requestSem = obtenerSemaforo(requestSemTok,0);
    printf("\nIdentificador del semaforo de consulta al servidor : %d\n",requestSem);
    responseSem = obtenerSemaforo(responseSemTok,0);
    printf("\nIdentificador del semaforo de respuesta de servidor: %d\n",responseSem);

    crearLista(&lista);
    leerArchivo(&fp, &lista);

    if (signal(SIGQUIT, cerrarServer) < 0) {
        printf("\nError al asociar la signal SIGQUIT con el handler.");
        return -1;
    }

    if (signal(SIGKILL, cerrarServer) < 0) {
        printf("\nError al asociar la signal SIGKILL con el handler.");
        return -1;
    }

    if (signal(SIGSTOP, cerrarServer) < 0) {
        printf("\nError al asociar la signal SIGSTOP con el handler.");
        return -1;
    }

    if (signal(SIGINT, cerrarServer) < 0) {
        printf("\nError al asociar la signal SIGINT con el handler.");
        return -1;
    }

    if (signal(SIGTERM, cerrarServer) < 0) {
        printf("\nError al asociar la signal SIGTERM con el handler.");
        return -1;
    }

    while (1) {
        pedirSemaforo(requestSem);

        switch (buffer->opcion) {
            case 1:
                printf("caso 1");
                if (ingresarMulta(buffer->multas[0].patente, buffer->multas[0].partido, buffer->multas[0].monto_total, buffer->multas[0].nombre_titular, &lista) == TODO_OK){
                    printf("Se ingreso la multa exitosamente.\n");
                    buffer->msg = "Se ingreso la multa exitosamente.\n";
                } else{
                    printf("La multa no se pudo generar, intente nuevamente.\n");
                    buffer->msg = "La multa no se pudo generar, intente nuevamente.\n";
                }

                devolverSemaforo(responseSem);
                break;
            case 2:
                printf("caso 2");
                if (registrosSuspender(buffer, &lista , buffer->multas[0].partido) == NOT_OK){
                    printf("No se encontraron registros a suspender.\n");
                    buffer->msg = "No se encontraron registros a suspender.\n";
                }
                devolverSemaforo(responseSem);
                break;
            case 3:
                if (saldarMulta(buffer->multas[0].patente, buffer->multas[0].partido, &lista) == NOT_OK){
                    printf("La patente ingresada no tiene multas a saldar.\n");
                    buffer->msg = "La patente ingresada no tiene multas a saldar.\n";
                }

                devolverSemaforo(responseSem);
                break;
            case 4:
                if(buscarMontoTotal(buffer, buffer->multas[0].patente, buffer->multas[0].partido, &lista) == NOT_OK){
                    printf("La patente ingresada no tiene multas pendientes.\n");
                    buffer->msg = "La patente ingresada no tiene multas pendientes.\n";
                }

                devolverSemaforo(responseSem);
                break;
            case 5:
                if (verMontoTotalInfractores (buffer, &lista, buffer->multas[0].partido) == NOT_OK){
                    printf("No hay patentes con multas pendientes.\n");
                    buffer->msg = "No hay patentes con multas pendientes.\n";
                }

                devolverSemaforo(responseSem);
                break;
            default:
                return NOT_OK;
        }
    }

    return TODO_OK;
}
