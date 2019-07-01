// Script Tp3-Ej4-Cliente

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h>

#define MAX         800
//#define SERVER_IP   127.0.0.1
#define PORT        8181
#define SA          struct sockaddr

#define NOT_OK      0
#define TODO_OK     1

int sockfd;

void SignalInterruptHandler(int n_signal) {
     write(sockfd, "7");
}

void operar(int sockfd) {
    // opcion del menu
    int code;

    //variables para operar
    char *patente = malloc(8);
    float monto;

    char buff[MAX];

    int n;

    memset(buff, 0, MAX);
    read(sockfd, buff, sizeof(buff));
    printf("%s", buff);
    fflush(stdin);

    memset(buff, 0, MAX);
    read(sockfd, buff, sizeof(buff));
    printf("%s", buff);
    fflush(stdin);

    // Ingreso partido
    memset(buff, 0, MAX);
    n = 0;
    while ((buff[n++] = getchar()) != '\n');
    buff[n-1] = '\0';
    write(sockfd, buff, sizeof(buff));

    // Leo el menu
    memset(buff, 0, MAX);
    read(sockfd, buff, sizeof(buff));
    printf("%s", buff);
    fflush(stdin);
    while (1) {
        // El servidor me pide la opcion
        memset(buff, 0, MAX);
        read(sockfd, buff, sizeof(buff));
        printf("%s", buff);
        fflush(stdin);

        // Ingreso la opcion y se la envio al servidor
        memset(buff, 0, MAX);
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));
        code = atoi(buff);

        while (code < 1 || code > 7){
            memset(buff, 0, MAX);
            read(sockfd, buff, sizeof(buff));
            printf("%s", buff);

            memset(buff, 0, MAX);
            n = 0;
            while ((buff[n++] = getchar()) != '\n');
            write(sockfd, buff, sizeof(buff));
            code = atoi(buff);
        }

        switch (code) {
            case 1:
                // Informo codigo 1 al server (ingresarMulta)
                write(sockfd, "1", sizeof(1));

                // Server me solicita patente
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                // Envio patente al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                // Server solicita monto
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                // Envio monto al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                // Server solicita nombre de titular
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                // Envio nombre de titular
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                // Leo resultado de alta de multa
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);
                break;
            case 2:
                // Envio opcion 2 al server (registrosASuspender)
                write(sockfd, "2", sizeof(1));

                // Leo los registros a suspender enviados por server
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);
                break;
            case 3:
                // Envio opcion 3 al server (saldarMulta)
                write(sockfd, "3", sizeof(1));

                // Server me solicita patente
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                // Envio patente al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                // Leo resultado del server
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                break;
            case 4:
                // Envio opcion 4 al server (Buscar monto de un infractor)
                write(sockfd, "4", sizeof(1));

                // Server solicita patente
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                // Envio patente al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                // Leo respuesta del server
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);

                break;
            case 5:
                // Envio opcion 5 al server (buscar todos los infractores y monto)
                write(sockfd, "5", sizeof(1));

                // Leo el resultado del server
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);
                break;
            case 6:
                // Envio opcion 6 al server (mostrar menu)
                write(sockfd, "6", sizeof(1));

                // Leo el resultado del server
                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);
                break;
            case 7:
                // Envio opcion 7 al server (salir)
                write(sockfd, "7", sizeof(1));
                printf("La sesion ha finalizado exitosamente.\n");

                return;
            default:
                // Envio opcion 7 al server (salir)
                write(sockfd, "7", sizeof(1));
                printf("Ocurrio un error inesperado y debio ser cerrada la conexion.\n");
                return;
        }
    }
}

int main() {
    int connfd;
    struct sockaddr_in servaddr, cli;

    signal(SIGINT, &SignalInterruptHandler);

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(NOT_OK);
    }

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(NOT_OK);
    }

    // function for chat
    operar(sockfd);

    // close the socket
    close(sockfd);

    return TODO_OK;
}
