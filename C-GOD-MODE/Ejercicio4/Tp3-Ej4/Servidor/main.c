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
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// Bibliotecas propias.
#include "info.h"
#include "list.h"
#include "functions.h"


#define PORT                        8181
#define MAX                         1025
#define SA                          struct sockaddr
#define MAX_PENDING_CONNECTIONS     3
#define MAX_CLIENT                  10

// Lista compartida por todos los clientes
t_list lista;

// Mensajes a clientes
char welcome[] = "Bienvenido al sistema de infracciones de transito del gobierno.\n";
char being_started[] = "Para comenzar ingrese el nombre del partido en el que opera: ";
char indicate_option[] = "\nIndique la opcion: ";
char invalid_option[] = "\nOpcion invalida, ingrese un numero entre 1 y 7: ";
char ask_patente[] = "Ingrese patente: ";
char ask_amount[] = "Ingrese monto de la multa: ";
char ask_titular_name[] = "Ingrese nombre del titular: ";
char fee_succesfull[] = "Se ingreso la multa exitosamente.\n";
char fee_unsuccesfull[] = "La multa no se pudo generar, intente nuevamente.\n";
char not_pending_fee[] = "La patente ingresada no tiene multas a saldar.\n";
char common_not_found_message[] = "No se encontraron infractores.\n";

void INThandler(int);
void operar(int, char*);

int main() {
    // Puntero a archivo de la BD
    FILE *fp;
    char *partido = malloc(25);

    // Controlar seniales enviadas como Ctrl+C
    signal(SIGINT, INThandler);
    // Creo la lista que estará compartida para todos los sockets
    crearLista(&lista);
    leerArchivo(&fp, &lista);


    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(NOT_OK);
    }
    else {
        printf("Socket successfully created...\n");
    }
    memset(&servaddr, 0, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(NOT_OK);
    }
    else{
        printf("Socket successfully binded..\n");
    }

    // Server listo para escuchar y verificacion
    if ((listen(sockfd, MAX_PENDING_CONNECTIONS)) != 0) {
        printf("Listen failed...\n");
        exit(NOT_OK);
    }
    else {
        printf("Server listening..\n");
    }
    len = sizeof(cli);

    while(1) {
        // Aceptar el paquete de datos del cliente y verificacion
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("Server acccept failed...\n");
            exit(NOT_OK);
        }
        else{
            printf("Server acccept the client %d...\n", connfd);
        }

        // cliente opera
        // Muestro mensaje de bienvenida
        send(connfd, welcome, sizeof(welcome), 0);

        // Leo partido del cliente
        memset(partido, 0, MAX);
        recv(connfd, partido, sizeof(partido), 0);

        // Normalizo nombre del partido
        normalizarCadena(partido, strlen(partido));

        printf("El cliente %d es del partido %s.\n", connfd, partido);
        // Funcion para que opere el cliente
        operar(connfd, partido);
       // Cierro cliente cuando finaliza de operar
        close(connfd);
    }

    // Cierro el socket de escucha cuando el servidor finaliza
    close(sockfd);

    return TODO_OK;
}


void INThandler(int sig) {
     char  c;

     printf("Realmente quieres cerrar el servidor? [s/n] ");
     c = getchar();
     if (c == 's' || c == 'S'){
          exit(0);
     }
     else {
          signal(SIGINT, INThandler);
     }
     getchar(); // Get new line character
}


void operar(int sockfd, char *partido) {
    // opcion del menu
    int code;

    //variables para operar
    char *patente = malloc(8);
    char *nombre_titular = malloc(25);
    char *aux;
    int wrong_option = 0;
    float monto;

    // buffer usado para dar las respuestas al client
    char buff[MAX];

    // Le envio menu al cliente
    mostrarMenu(sockfd);
    fflush(stdout);
    while (1) {

        // Le pido a cliente que ingrese una opcion
        memset(buff, 0, MAX);
        send(sockfd, indicate_option, sizeof(indicate_option), 0);
        fflush(stdout);

        // Leo opcion ingresada por el cliente
        memset(buff, 0, MAX);
        recv(sockfd, buff, sizeof(buff), 0);
        code = atoi(buff);

        printf("El cliente %d selecciono la opcion %d.\n", sockfd, code);

        while (code < 1 || code > 7){
            send(sockfd, invalid_option, sizeof(invalid_option), 0);

            memset(buff, 0, MAX);
            recv(sockfd, buff, sizeof(buff), 0);
            code = atoi(buff);

            if (code == 0){
                wrong_option++;
            } else {
                wrong_option = 0;
            }
            if (wrong_option == 3){
                code = 7;
            }
            printf("El cliente %d selecciono la opcion %d.\n", sockfd, code);
        }

        fflush(stdout);

        switch (code) {
            case 1:
                // Le pido al usuario que ingrese la patente
                send(sockfd, ask_patente, sizeof(ask_patente), 0);
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                strcpy(patente, buff);

                // Le pido al usuario que ingrese el monto de la multa
                send(sockfd, ask_amount, sizeof(ask_amount), 0);
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                monto = atof(buff);

                // Le pido al usuario que ingrese el nombre del titular
                send(sockfd, ask_titular_name, sizeof(ask_titular_name), 0);
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                strcpy(nombre_titular, buff);

                if (ingresarMulta(patente, partido, nombre_titular, monto, &lista) == TODO_OK){
                    send(sockfd, fee_succesfull, sizeof(fee_succesfull), 0);
                } else{
                    send(sockfd, fee_unsuccesfull, sizeof(fee_unsuccesfull), 0);
                }
                fflush(stdout);
                break;
            case 2:
                memset(buff, 0, MAX);

                if (registrosSuspender(&lista ,partido, buff) == TODO_OK) {
                    send(sockfd, buff, sizeof(buff), 0);
                } else{
                    send(sockfd, common_not_found_message, sizeof(common_not_found_message), 0);
                }

                fflush(stdout);
                break;
            case 3:
                send(sockfd, ask_patente, sizeof(ask_patente), 0);

                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                strcpy(patente, buff);

                if (saldarMulta(patente, partido, &lista) == NOT_OK){
                    send(sockfd, not_pending_fee, sizeof(not_pending_fee), 0);
                }
                fflush(stdout);
                break;
            case 4:
                send(sockfd, ask_patente, sizeof(ask_patente), 0);

                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                strcpy(patente, buff);

                memset(buff, 0, MAX);

                printf("El cliente %d busca infractor con patente %s.\n", sockfd, patente);

                if (buscarMontoTotal(patente, partido, &lista, buff) == TODO_OK) {
                    send(sockfd, buff, sizeof(buff), 0);
                } else{
                    send(sockfd, common_not_found_message, sizeof(common_not_found_message), 0);
                }

                fflush(stdout);
                break;
            case 5:
                memset(buff, 0, MAX);
                if (verMontoTotalInfractores (&lista, partido, buff) == TODO_OK){
                    send(sockfd, buff, sizeof(buff), 0);
                } else{
                    send(sockfd, common_not_found_message, sizeof(common_not_found_message), 0);
                }

                fflush(stdout);
                break;
            case 6:
                mostrarMenu(sockfd);
                fflush(stdout);
                break;
            case 7:
                printf("El cliente %d cierra la sesion exitosamente.\n", sockfd);
                return;
                break;
            default:
                printf("Se cierra la sesion del cliente %d debido a un problema.\n", sockfd);
                return;
        }
    }
}
