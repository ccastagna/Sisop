// Script Tp3-Ej4-Servidor

// Trabajo práctico 3
// Ejercicio 4
// Primera-Reentrega

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


#define MAX                         1025
#define SA                          struct sockaddr
#define MAX_PENDING_CONNECTIONS     3
#define MAX_CLIENT                  10

// Lista compartida por todos los clientes
t_list lista;

// Mensajes a clientes
char welcome[] = "Bienvenido al sistema de infracciones de transito del gobierno.\n\0";
char indicate_option[] = "\nIndique la opcion: \0";
char invalid_option[] = "\nOpcion invalida, ingrese un numero entre 1 y 7: \0";
char ask_patente[] = "\nIngrese patente: \0";
char ask_amount[] = "\nIngrese monto de la multa: \0";
char ask_titular_name[] = "\nIngrese nombre del titular: \0";
char fee_succesfull[] = "\nSe ingreso la multa exitosamente.\n\0";
char fee_unsuccesfull[] = "\nLa multa no se pudo generar, intente nuevamente.\n\0";
char not_pending_fee[] = "\nLa patente ingresada no tiene multas a saldar.\n\0";
char common_not_found_message[] = "\nNo se encontraron infractores.\n\0";

void INThandler(int);
void operar(int, char*);

int main(int argc, char *argv[]) {
    // Puntero a archivo de la BD
    FILE *fp;

    // Declarcion de variables
    char *partido = malloc(25);
    *partido = '\0';
    int sockfd, connfd, len, portno;
    struct sockaddr_in servaddr, cli;

    // Controlar seniales enviadas como Ctrl+C
    signal(SIGINT, INThandler);

    if (argc < 2){
	fprintf(stderr, "Uso %s <puerto_servidor>\n", argv[0]);
	exit(NOT_OK);
    }
    
    // mapeo a variables los parametros recibidos
    portno = atoi(argv[1]);

    // Creo la lista que estará compartida para todos los sockets
    crearLista(&lista);
    leerArchivo(&fp, &lista);

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
    servaddr.sin_port = htons(portno);

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
        printf("Server listening on port %d..\n", portno);
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
        write(connfd, welcome, sizeof(welcome));

        // Leo partido del cliente
        memset(partido, 0, sizeof(partido));
        read(connfd, partido, sizeof(partido));

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

    return (int)TODO_OK;
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
    *patente = '\0';
    char *nombre_titular = malloc(25);
    *nombre_titular = '\0';
    float monto;

    // buffer usado para dar las respuestas al cliente
    char buff[MAX];

    // Le envio menu al cliente
    mostrarMenu(sockfd);
    fflush(stdout);
    while (1) {
        sleep(1);
        // Le pido a cliente que ingrese una opcion
        memset(buff, 0, MAX);
        write(sockfd, indicate_option, sizeof(indicate_option));
        fflush(stdout);

        // Leo opcion ingresada por el cliente
        memset(buff, 0, MAX);
        read(sockfd, buff, sizeof(buff));
        code = atoi(buff);

        // Si el codigo es 0 es porque el cliente salio de la consola con una señal no tenida en cuenta.
        // Asigno codigo 7 para entrar en flujo de cierre de conexion.
        if (code == 0){
            code = 7;
        }

        printf("El cliente %d selecciono la opcion %d.\n", sockfd, code);

        fflush(stdout);

        switch (code) {
            case 1:
                // Le pido al usuario que ingrese la patente
                write(sockfd, ask_patente, sizeof(ask_patente));
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                strcpy(patente, buff);

                // Le pido al usuario que ingrese el monto de la multa
                write(sockfd, ask_amount, sizeof(ask_amount));
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                monto = atof(buff);

                // Le pido al usuario que ingrese el nombre del titular
                write(sockfd, ask_titular_name, sizeof(ask_titular_name));
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                strcpy(nombre_titular, buff);

                if (ingresarMulta(patente, partido, nombre_titular, monto, &lista) == TODO_OK){
                    write(sockfd, fee_succesfull, sizeof(fee_succesfull));
                } else{
                    write(sockfd, fee_unsuccesfull, sizeof(fee_unsuccesfull));
                }
                fflush(stdout);
                break;
            case 2:
                memset(buff, 0, MAX);

                if (registrosSuspender(&lista ,partido, buff) == TODO_OK) {
                    write(sockfd, buff, sizeof(buff));
                } else{
                    write(sockfd, common_not_found_message, sizeof(common_not_found_message));
                }

                fflush(stdout);
                break;
            case 3:
                write(sockfd, ask_patente, sizeof(ask_patente));

                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                strcpy(patente, buff);

                if (saldarMulta(patente, partido, &lista) == NOT_OK){
                    write(sockfd, not_pending_fee, sizeof(not_pending_fee));
                }
                fflush(stdout);
                break;
            case 4:
                // Le pido al cliente que ingrese patente
                write(sockfd, ask_patente, sizeof(ask_patente));

                // Leo la patente enviada por el cliente
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                strcpy(patente, buff);

                memset(buff, 0, MAX);
                printf("El cliente %d busca infractor con patente %s.\n", sockfd, patente);

                // Le indico al usuario lo que debe la patente o le indico que no existe.
                if (buscarMontoTotal(patente, partido, &lista, buff) == TODO_OK) {
                    write(sockfd, buff, sizeof(buff));
                } else{
                    write(sockfd, common_not_found_message, sizeof(common_not_found_message));
                }

                fflush(stdout);
                break;
            case 5:
                memset(buff, 0, MAX);

                // Si hay infractores para el partido los muestro, sino indico que no hay.
                if (verMontoTotalInfractores (&lista, partido, buff) == TODO_OK){
                    write(sockfd, buff, sizeof(buff));
                } else{
                    write(sockfd, common_not_found_message, sizeof(common_not_found_message));
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
