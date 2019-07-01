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

#define MAX 800
#define PORT 8080
#define SA struct sockaddr

#define NOT_OK 0
#define TODO_OK 1

void operar(int sockfd)
{
    // opcion del menu
    int code;

    //variables para operar
    char *patente = malloc(8);
    float monto;

    char buff[MAX];

    int n;

    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s", buff);

    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s", buff);

    bzero(buff, sizeof(buff));
    n = 0;
    while ((buff[n++] = getchar()) != '\n');
    write(sockfd, buff, sizeof(buff));

    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s", buff);
    fflush(stdin);
    while (1) {
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("%s", buff);

        bzero(buff, sizeof(buff));
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));

        code = atoi(buff);

        while (code < 1 || code > 7){
            bzero(buff, sizeof(buff));
            read(sockfd, buff, sizeof(buff));
            printf("%s", buff);

            bzero(buff, sizeof(buff));
            n = 0;
            while ((buff[n++] = getchar()) != '\n');
            write(sockfd, buff, sizeof(buff));
            code = atoi(buff);
        }

        switch (code) {
            case 1:
                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                bzero(buff, sizeof(buff));
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                bzero(buff, sizeof(buff));
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                bzero(buff, sizeof(buff));
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);
                break;
            case 2:
                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);
                break;
            case 3:
                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                bzero(buff, sizeof(buff));
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                break;
            case 4:
                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);

                bzero(buff, sizeof(buff));
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));

                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);

                break;
            case 5:
                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);
                break;
            case 6:
                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                fflush(stdin);
                break;
            case 7:
                bzero(buff, sizeof(buff));
                read(sockfd, buff, sizeof(buff));
                printf("%s", buff);
                return;
                break;
            default:
                bzero(buff, sizeof(buff));
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                write(sockfd, buff, sizeof(buff));
                printf("Ocurrio un error inesperado y debio ser cerrada la conexion.\n");
                return;
        }
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

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
