// Script Tp3-Ej5-Server

// Trabajo pr�ctico 3
// Ejercicio 5
// Primera Reentrega

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

int main(int argc, char *argv[])
{

    if(argc == 2) {
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-H") || !strcmp(argv[1], "-?") || !strcmp(argv[1], "-a")) {
                mostrarAyuda(argv[0]);
                exit((int)TODO_OK);
        }
    }

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
    printf("\nIdentificador del semaforo de respuesta de servidor: %d\n\n\n",responseSem);

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

    if (signal(SIGSEGV, cerrarServer) < 0) {
        printf("\nError al asociar la signal SIGSEGV con el handler.");
        return -1;
    }

    buffer->cantMultas = 0;
    while (1) {
        pedirSemaforo(requestSem);

        switch (buffer->opcion) {
            case 1:
                printf("Ingresando multa para la patente: %s  en el partido: %s \n\n", buffer->multas[0].patente, buffer->multas[0].partido);

                if (ingresarMulta(buffer->multas[0].patente, buffer->multas[0].partido, buffer->multas[0].monto_total, buffer->multas[0].nombre_titular, &lista) == (int)TODO_OK){
                    printf("Se ingreso la multa exitosamente.\n\n\n");
                    strcpy(buffer->msg, "Se ingreso la multa exitosamente.\n\n\n");

                } else {
                    printf("La multa no se pudo generar, intente nuevamente.\n\n\n");
                    strcpy(buffer->msg, "La multa no se pudo generar, intente nuevamente.\n\n\n");
                }

                devolverSemaforo(responseSem);
                break;
            case 2:
                printf("Buscando registros a suspender... \n\n");

                if (registrosSuspender(buffer, &lista , buffer->multas[0].partido) == (int)NOT_OK){
                    printf("No se encontraron registros a suspender.\n\n\n");
                    strcpy(buffer->msg, "No se encontraron registros a suspender.\n\n\n");

                } else {
                    printf("Se encontraron los registros a suspender.\n\n\n");
                }

                devolverSemaforo(responseSem);
                break;
            case 3:
                printf("Saldando multa para la patente: %s  en el partido: %s \n\n",buffer->multas[0].patente, buffer->multas[0].partido);

                if (saldarMulta(buffer->multas[0].patente, buffer->multas[0].partido, &lista) == (int)NOT_OK){
                    printf("La patente ingresada no tiene multas a saldar.\n\n\n");
                    strcpy(buffer->msg, "La patente ingresada no tiene multas a saldar.\n\n\n");

                } else {
                    printf("Las multas para la patente: %s estan saldadas correctamente...\n\n\n", buffer->multas[0].patente);
                    strcpy(buffer->msg, "Multas saldadas correctamente...\n\n\n");
                }

                devolverSemaforo(responseSem);
                break;
            case 4:
                printf("Buscando monto total para la patente: %s  en el partido: %s \n\n",buffer->multas[0].patente, buffer->multas[0].partido);

                if(buscarMontoTotal(buffer, buffer->multas[0].patente, buffer->multas[0].partido, &lista) == (int)NOT_OK){
                    printf("La patente ingresada no tiene multas pendientes.\n\n\n");
                    strcpy(buffer->msg, "La patente ingresada no tiene multas pendientes.\n\n\n");
                } else {
                    printf("Las multas para la patente: %s  se encontraron correctamente. \n\n\n", buffer->multas[0].patente);
                }

                devolverSemaforo(responseSem);
                break;
            case 5:
                printf("Buscando monto total a pagar de todos los infractores del partido: %s \n\n", buffer->multas[0].partido);

		        if (verMontoTotalInfractores (buffer, &lista, buffer->multas[0].partido) == (int)NOT_OK){
                    printf("No hay patentes con multas pendientes.\n\n\n");
                    strcpy(buffer->msg, "No hay patentes con multas pendientes.\n\n\n");
                } else {
                    printf("Los montos totales de todos los infractores se encontraron con exito.\n\n\n");
                }

                devolverSemaforo(responseSem);
                break;
            default:
                return (int)NOT_OK;
        }
    }

    return (int)TODO_OK;
}
