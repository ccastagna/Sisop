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

#define MAX         1025
#define SA          struct sockaddr

#define NOT_OK      0
#define TODO_OK     1

int sockfd;

void SignalInterruptHandler(int);
void operar(int);

int main(int argc, char *argv[]) {
    int connfd, portno;
    struct sockaddr_in servaddr;
    struct hostent *server;
    char buff[60];

    signal(SIGINT, &SignalInterruptHandler);

    if (argc < 4) {
        fprintf(stderr,"Uso %s <ip_servidor> <puerto_servidor> <partido>\n", argv[0]);
        exit(NOT_OK);
    }

    portno = atoi(argv[2]);

    size_t len = strlen(argv[3]);
    char *partido = malloc(len+1);
    strcpy(partido, argv[3]);

    printf("partido: %s\n\n", partido);

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


    char sarasa[] = "Lanus";

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(NOT_OK);
    }

    // Leo bienvenida del servidor
    memset(buff, 0, MAX);
    recv(sockfd, buff, sizeof(buff), 0);
    printf("%s", buff);
    fflush(stdin);

    // Le informo al servidor de que partido soy
    //write(sockfd, argv[3], sizeof(argv[3]));
    send(sockfd, sarasa, sizeof(sarasa), 0);

    // funcion para operar
    operar(sockfd);

    // close the socket
    close(sockfd);

    return TODO_OK;
}

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

    // Leo el menu
    memset(buff, 0, MAX);
    recv(sockfd, buff, sizeof(buff), 0);
    printf("%s", buff);
    fflush(stdin);
    while (1) {
        // El servidor me pide la opcion
        memset(buff, 0, MAX);
        recv(sockfd, buff, sizeof(buff), 0);
        printf("%s", buff);
        fflush(stdin);

        // Ingreso la opcion y se la envio al servidor
        memset(buff, 0, MAX);
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        send(sockfd, buff, sizeof(buff), 0);
        code = atoi(buff);

        while (code < 1 || code > 7){
            memset(buff, 0, MAX);
            recv(sockfd, buff, sizeof(buff), 0);
            printf("%s", buff);

            memset(buff, 0, MAX);
            n = 0;
            while ((buff[n++] = getchar()) != '\n');
            send(sockfd, buff, sizeof(buff), 0);
            code = atoi(buff);
        }

        // Informo codigo al server
        send(sockfd, buff, sizeof(1), 0);

        switch (code) {
            case 1: // (ingresarMulta)

                // Server me solicita patente
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);

                // Envio patente al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                send(sockfd, buff, sizeof(buff), 0);

                // Server solicita monto
                memset(buff, 0, MAX);
                send(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);

                // Envio monto al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                send(sockfd, buff, sizeof(buff), 0);

                // Server solicita nombre de titular
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);

                // Envio nombre de titular
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                send(sockfd, buff, sizeof(buff), 0);

                // Leo resultado de alta de multa
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);
                fflush(stdin);
                break;
            case 2: // (registrosASuspender)

                // Leo los registros a suspender enviados por server
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);
                fflush(stdin);
                break;
            case 3: // (saldarMulta)

                // Server me solicita patente
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);

                // Envio patente al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                send(sockfd, buff, sizeof(buff), 0);

                // Leo resultado del server
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);

                break;
            case 4: // (Buscar monto de un infractor)

                // Server solicita patente
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);

                // Envio patente al server
                memset(buff, 0, MAX);
                n = 0;
                while ((buff[n++] = getchar()) != '\n');
                send(sockfd, buff, sizeof(buff), 0);

                // Leo respuesta del server
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);
                fflush(stdin);

                break;
            case 5: // (buscar todos los infractores y monto)

                // Leo el resultado del server
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);
                fflush(stdin);
                break;
            case 6: // (mostrar menu)

                // Leo el resultado del server
                memset(buff, 0, MAX);
                recv(sockfd, buff, sizeof(buff), 0);
                printf("%s", buff);
                fflush(stdin);
                break;
            case 7: // (salir)

                printf("La sesion ha finalizado exitosamente.\n");
                return;
            default: // (salir)
                send(sockfd, "7", sizeof(1), 0);
                printf("Ocurrio un error inesperado y debio ser cerrada la conexion.\n");
                return;
        }
    }
}
