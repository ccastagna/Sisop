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
#include <string.h>

// Bibliotecas propias.
#include "functions.h"


int main()
{
    int code;
    int i;
    //t_list lista;
    char partido[100], patente[100], nombre_titular[100];
    float monto;

    key_t          ShmKEY;
    int            ShmID;
    t_buffer  *buffer;
    int clientSem, requestSem, responseSem;
    
    ShmKEY = ftok("../Server/", 'x');
    ShmID = shmget(ShmKEY, sizeof(t_buffer), 0666);
    if (ShmID < 0) {
        printf("*** shmget error (client) ***\n");
        exit(1);
    }    

    key_t clientSemTok = ftok("../Server/",1000);
    key_t requestSemTok = ftok("../Server/",1001);
    key_t responseSemTok = ftok("../Server/",1002);

    clientSem = obtenerSemaforo(clientSemTok);
    printf("\nIdentificador del semaforo de cliente: %d\n",clientSem);
    requestSem = obtenerSemaforo(requestSemTok);
    printf("\nIdentificador del semaforo de consultas al servidor : %d\n",requestSem);
    responseSem = obtenerSemaforo(responseSemTok);
    printf("\nIdentificador del semaforo de respuesta de servidor: %d\n",responseSem);
    
    if(clientSem < 0 || requestSem < 0 || responseSem < 0){
        printf("\nERROR DE CONEXION...\n");
        printf("PRIMERO SE DEBE EJECUTAR EL SERVIDOR.\n\n");
        exit(2);
    }

    buffer = (t_buffer *) shmat(ShmID, NULL, 0);
    printf("buffer by shmat %p \n", buffer);  

    printf("Ingrese el partido de su sede: ");

    do{
	memset(partido, 0, 100);
	strcpy(partido, "\0");
        i = 0;
        while((partido[i++] = getchar()) != '\n');
    }while(strlen(partido) > 20);

    partido[i-1] = '\0';

    while (1) {     
        pedirSemaforo(clientSem);
        
        strcpy(buffer->multas[0].partido, partido);

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
		getchar();
                do {
		    printf("Ingrese la patente: ");
		    memset(patente, 0, 100);
                    i = 0;
                    while((patente[i++] = getchar()) != '\n');
                }while(strlen(patente) > 8 || strlen(patente) < 2);
		patente[i-1] = '\0';

                fflush(stdin);
                strcpy(buffer->multas[0].patente, patente);


                printf("Ingrese monto de la multa: ");
                fflush(stdin);
                scanf("%f", &buffer->multas[0].monto_total);
                fflush(stdin);

                printf("Ingrese el nombre del titular: ");
                fflush(stdin);

		getchar();
                do{
	            memset(nombre_titular, 0, 100);
                    i=0;
                    while((nombre_titular[i++] = getchar()) != '\n');
                }while(strlen(nombre_titular) > 8);
                nombre_titular[i-1] = '\0';

                fflush(stdin);
                
                strcpy(buffer->multas[0].nombre_titular, nombre_titular);
                
                devolverSemaforo(requestSem);
                pedirSemaforo(responseSem);

                printf("%s",buffer->msg);

                limpiarBuffer(buffer);
                devolverSemaforo(clientSem);
                break;
            case 2:

                devolverSemaforo(requestSem);
                pedirSemaforo(responseSem);

                for(i=0; i <= buffer->cantMultas ; i++ ){
                    printf("%s\n", buffer->multas[i].patente);
                }

                printf("%s",buffer->msg);

                limpiarBuffer(buffer);
                devolverSemaforo(clientSem);
                break;
            case 3:

                printf("Ingrese la patente: ");
                scanf("%7s", buffer->multas[0].patente);
                
                devolverSemaforo(requestSem);
                pedirSemaforo(responseSem);

                printf("%s",buffer->msg);
               
                limpiarBuffer(buffer);
                devolverSemaforo(clientSem);
                break;
            case 4:

                printf("Ingrese la patente: ");
                scanf("%7s", buffer->multas[0].patente);
                
                devolverSemaforo(requestSem);
                pedirSemaforo(responseSem);

                printf("%s\t%.2f\n", buffer->multas[0].patente, buffer->multas[0].monto_total);
                printf("%s",buffer->msg);

                limpiarBuffer(buffer);
                devolverSemaforo(clientSem);
                break;
            case 5:
                devolverSemaforo(requestSem);
                pedirSemaforo(responseSem);

                for(i=0; i < buffer->cantMultas ; i++ ){
                    printf("%s\t%.2f\n", buffer->multas[i].patente, buffer->multas[i].monto_total);
                }
                printf("%s",buffer->msg);
                
                limpiarBuffer(buffer);
                devolverSemaforo(clientSem);
                break;
            case 6:
                mostrarMenu();
                break;
            case 7:
                shmdt((void *) buffer);
                devolverSemaforo(clientSem);
                exit(0);
                break;
            default:
                mostrarMenu();
                break;
        }
    }

    return (int)TODO_OK;
}
