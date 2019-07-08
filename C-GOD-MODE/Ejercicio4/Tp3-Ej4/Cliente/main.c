// Script Tp3-Ej4-Cliente

// Trabajo práctico 3
// Ejercicio 4
// Primera Reentrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869

// Bibliotecas pre-compiladas.

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>

// Bibliotecas del usuario.
#include "info.h"
#include "functions.h"

#define MAX         1024
#define SA          struct sockaddr

#define NOT_OK      -1
#define TODO_OK     0

int sockfd;
char invalid_option[] = "Opcion invalida, ingrese un numero entre 1 y 7: ";

void SignalInterruptHandler(int);
void operar(int);

int main(int argc, char *argv[]) {
    int portno;
    struct sockaddr_in servaddr;
    struct hostent *server;
    char buff[60];
    int identificador;
    signal(SIGINT, &SignalInterruptHandler);
    if(argc == 2) {
	printf("%d\t", argc);
	printf("%s\t", argv[0]);
	printf("%s\t", argv[1]);
	printf("%s\n\n", argv[2]);
	if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-H") || !strcmp(argv[1], "-?")) {
		mostrarAyuda(argv[0]);
		exit(TODO_OK);
    	}
    }

    if (argc < 4) {
        fprintf(stderr,"\nIngrese %s -h para obtener ayuda.\n\n", argv[0]);
        exit(TODO_OK);
    }

    portno = atoi(argv[2]);

    // partido al que pertenece el cliente
    char partido[sizeof(argv[3] + 2)];
    strcpy(partido, argv[3]);
    strcat(partido, "\0");

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(NOT_OK);
    }
    server = gethostbyname(argv[1]);

    bzero(&servaddr, sizeof(servaddr));

    // asigno IP, PORT
    servaddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length);
    servaddr.sin_port = htons(portno);



    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(NOT_OK);
    }

    // Leo bienvenida del servidor
    memset(buff, 0, MAX);
    leerMensaje(sockfd, &identificador, buff);
    printf("%s", buff);
    fflush(stdin);

    // Le informo al servidor de que partido soy
    escribirMensaje(sockfd, id_partido, partido, strlen(partido));

    // funcion para operar
    operar(sockfd);

    // close the socket
    // close(sockfd);

    return (int)TODO_OK;
}

void SignalInterruptHandler(int n_signal) {
     escribirMensaje(sockfd, id_opcion, "7\0", 2);
}

void operar(int sockfd) {
    // opcion del menu
    int code;

    // identificador para recibir cabecera del mensaje
    int identificador;

    // buffer para comunicacion
    char buff[MAX];

    // indice para leer buffer de consola
    int n;

    // Leo el menu
    memset(buff, 0, MAX);
    leerMensaje(sockfd, &identificador, buff);
    printf("%s", buff);
    fflush(stdin);
    while (1) {
        // El servidor me pide la opcion
        memset(buff, 0, MAX);
        leerMensaje(sockfd, &identificador, buff);
        printf("%s", buff);
        fflush(stdin);

        // Ingreso la opcion y se la envio al servidor
        memset(buff, 0, MAX);
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        code = atoi(buff);


        while (code < 1 || code > 7){
            //memset(buff, 0, MAX);
            //read(sockfd, buff, sizeof(buff));
            printf("%s", invalid_option);

            memset(buff, 0, MAX);
            n = 0;
            while ((buff[n++] = getchar()) != '\n');
            code = atoi(buff);
        }

        // Informo codigo al server
        buff[n-1] = '\0';
	escribirMensaje(sockfd, id_opcion, buff, strlen(buff));

        switch (code) {
            case 1: // (ingresarMulta)
                // Server me solicita patente
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);

                // Envio patente al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
		buff[n-1] = '\0';
		escribirMensaje(sockfd, id_patente, buff, strlen(buff));

                // Server solicita monto
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);

                // Envio monto al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                buff[n-1] = '\0';
		escribirMensaje(sockfd, id_monto, buff, strlen(buff));

                // Server solicita nombre de titular
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);

                // Envio nombre de titular
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
		buff[n-1] = '\0';
                escribirMensaje(sockfd, id_nombre_titular, buff, strlen(buff));

                // Leo resultado de alta de multa
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);
                fflush(stdin);
                break;
            case 2: // (registrosASuspender)
                // Leo los registros a suspender enviados por server
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);
                fflush(stdin);
                break;
            case 3: // (saldarMulta)
                // Server me solicita patente
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);

                // Envio patente al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
		buff[n-1] = '\0';
                escribirMensaje(sockfd, id_patente, buff, strlen(buff));

                // Leo resultado del server
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);
                break;
            case 4: // (Buscar monto de un infractor)
                // Server solicita patente
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);

                // Envio patente al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                buff[n-1] = '\0';
                escribirMensaje(sockfd, id_patente, buff, strlen(buff));

                // Leo respuesta del server
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);
                fflush(stdin);
                break;
            case 5: // (buscar todos los infractores y monto)
                // Leo el resultado del server
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);
                fflush(stdin);
                break;
            case 6: // (mostrar menu)
                // Leo el resultado del server
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                printf("%s", buff);
                fflush(stdin);
                break;
            case 7: // (salir)
                escribirMensaje(sockfd, id_opcion, "7\0", 2);
                printf("La sesion ha finalizado exitosamente.\n");
                return;
            default: // (salir)
                escribirMensaje(sockfd, id_opcion, "7\0", 2);
                printf("Ocurrio un error inesperado y debio ser cerrada la conexion.\n");
                return;
        }
    }
}
