// Script Tp3-Ej4-Servidor

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
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

// Bibliotecas propias.
#include "info.h"
#include "list.h"
#include "functions.h"

#define MAX 1000
#define PORT 8080
#define SA struct sockaddr

#define MAX_WAIT_CALL 5

// Lista compartida por todos los threads
t_list lista;

void func(int sockfd) {
    // opcion del menu
    int code;

    //variables para operar
    char *partido = malloc(20);
    char *patente = malloc(8);
    char *nombre_titular = malloc(25);
    float monto;

    // buffer usado para dar las respuestas al client
    char buff[MAX];

    char welcome[] = "Bienvenido al sistema de infracciones de transito del gobierno.\n";
    char being_started[] = "Para comenzar ingrese el nombre del partido en el que opera: ";

    write(sockfd, welcome, sizeof(welcome));
    write(sockfd, being_started, sizeof(being_started));

    bzero(buff, MAX);
    read(sockfd, buff, sizeof(buff));
    strcpy(partido, buff);
    normalizarCadena(partido, strlen(partido));

    printf("El cliente %d es del partido %s.\n", sockfd, partido);
    mostrarMenu(sockfd);

    while (1) {
        bzero(buff, MAX);
        write(sockfd, "Indique la opcion: ");

        bzero(buff, MAX);
        read(sockfd, buff, sizeof(buff));
        code = atoi(buff);

        printf("El cliente %d selecciono la opcion %d.\n", sockfd, code);

        while (code < 1 || code > 7){
            write(sockfd, "Opcion invalida, ingrese un numero entre 1 y 7: ");

            bzero(buff, MAX);
            read(sockfd, buff, sizeof(buff));
            code = atoi(buff);

            printf("El cliente %d selecciono la opcion %d.\n", sockfd, code);
        }

        switch (code) {
            case 1:
                write(sockfd, "Ingrese la patente: ");

                bzero(buff, MAX);
                read(sockfd, buff, sizeof(buff));
                strcpy(patente, buff);


                write(sockfd, "Ingrese monto de la multa: ");

                bzero(buff, MAX);
                read(sockfd, buff, sizeof(buff));
                monto = atof(buff);

                write(sockfd, "Ingrese el nombre del titular: ");

                bzero(buff, MAX);
                read(sockfd, buff, sizeof(buff));
                strcpy(nombre_titular, buff);

                if (ingresarMulta(patente, partido, nombre_titular, monto, &lista) == TODO_OK){
                    write(sockfd, "Se ingreso la multa exitosamente.\n");
                } else{
                    write(sockfd, "La multa no se pudo generar, intente nuevamente.\n");
                }
                break;
            case 2:
                if (registrosSuspender(&lista ,partido, sockfd) == NOT_OK){
                    write(sockfd, "No se encontraron registros a suspender.\n");
                }
                break;
            case 3:
                write(sockfd, "Ingrese la patente: ");

                bzero(buff, MAX);
                read(sockfd, buff, sizeof(buff));
                strcpy(patente, buff);

                if (saldarMulta(patente, partido, &lista) == NOT_OK){
                    write(sockfd, "La patente ingresada no tiene multas a saldar.\n");
                }
                break;
            case 4:
                write(sockfd, "Ingrese la patente: ");

                bzero(buff, MAX);
                read(sockfd, buff, sizeof(buff));
                strcpy(patente, buff);

                if(buscarMontoTotal(patente, partido, &lista, sockfd) == NOT_OK){
                    write(sockfd, "La patente ingresada no tiene multas pendientes.\n");
                }
                break;
            case 5:
                if (verMontoTotalInfractores (&lista, partido, sockfd) == NOT_OK){
                    write(sockfd, "No hay patentes con multas pendientes.\n");
                }
                break;
            case 6:
                mostrarMenu(sockfd);
                break;
            case 7:
                write(sockfd, "Sesion finalizada.\n");
                printf("El cliente %d cierra la sesion exitosamente.\n", sockfd);
                return;
                break;
            default:
                printf("Se cierra la sesion del cliente %d debido a un problema.\n", sockfd);
                return;
        }
    }
}

int main()
{
    // Puntero a archivo de la BD
    FILE *fp;

    // Creo la lista que estará compartida para todos los sockets
    crearLista(&lista);
    leerArchivo(&fp, &lista);

    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else {
        printf("Socket successfully created...\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else{
        printf("Socket successfully binded..\n");
    }

    // Server listo para escuchar y verificacion
    if ((listen(sockfd, MAX_WAIT_CALL)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else {
        printf("Server listening..\n");
    }

    while(1) {
        // Aceptar el paquete de datos del cliente y verificacion
        len = sizeof(cli);
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("Server acccept failed...\n");
            exit(0);
        }
        else
            printf("Server acccept the client...\n");

        // Funcion para que opere el cliente
        func(connfd);

        // Cierro cliente cuando finaliza de operar
        close(connfd);
    }

    // Cierro el socket de escucha cuando el servidor finaliza
    close(sockfd);

    return TODO_OK;
}
