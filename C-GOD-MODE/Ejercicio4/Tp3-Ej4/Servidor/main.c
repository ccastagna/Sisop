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


#define SERVER_IP       "172.0.0.1"
#define CLIENT_IP       "172.0.0.1"
#define PORT            8181

#define MAX 500
#define BUF_LEN         500

#define SA struct sockaddr

#define MAX_WAIT_CALL 10

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


void operar(int sockfd) {
    // opcion del menu
    int code;

    //variables para operar
    char *partido = malloc(20);
    char *patente = malloc(8);
    char *nombre_titular = malloc(25);
    char *aux;
    int wrong_option = 0;
    float monto;

    // buffer usado para dar las respuestas al client
    char buff[MAX];


    // Muestro mensaje de bienvenida y pido partido
    write(sockfd, welcome, sizeof(welcome));
    fflush(stdin);
    write(sockfd, being_started, sizeof(being_started));
    fflush(stdin);

    // Leo partido del cliente
    memset(buff, 0, MAX);
    read(sockfd, buff, sizeof(buff));
    strcpy(partido, buff);

    // Normalizo nombre del partido
    normalizarCadena(partido, strlen(partido));

    printf("El cliente %d es del partido %s.\n", sockfd, partido);

    // Le envio menu al cliente
    mostrarMenu(sockfd);
    fflush(stdout);
    while (1) {
        // Le pido a cliente que ingrese una opcion
        memset(buff, 0, MAX);
        write(sockfd, indicate_option, sizeof(indicate_option));

        // Leo opcion ingresada por el cliente
        memset(buff, 0, MAX);
        read(sockfd, buff, sizeof(buff));
        printf("%s", buff);
        code = atoi(buff);

        printf("El cliente %d selecciono la opcion %d.\n", sockfd, code);

        while (code < 1 || code > 7){
            write(sockfd, invalid_option, sizeof(invalid_option));

            memset(buff, 0, MAX);
            read(sockfd, buff, sizeof(buff));
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
                aux = registrosSuspender(&lista ,partido);
                strcpy(buff, aux);
                write(sockfd, buff, sizeof(buff));
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
                write(sockfd, ask_patente, sizeof(ask_patente));

                memset(buff, 0, MAX);
                read(sockfd, buff, sizeof(buff));
                strcpy(patente, buff);

                aux = buscarMontoTotal(patente, partido, &lista);
                strcpy(buff, aux);
                write(sockfd, buff, sizeof(buff));
                fflush(stdout);
                break;
            case 5:
                aux = verMontoTotalInfractores (&lista, partido);
                strcpy(buff, aux);
                write(sockfd, buff, sizeof(buff));
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

int main() {
    // Puntero a archivo de la BD
    FILE *fp;

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
    bzero(&servaddr, sizeof(servaddr));

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
    if ((listen(sockfd, MAX_WAIT_CALL)) != 0) {
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

        // Funcion para que opere el cliente
        operar(connfd);

        // Cierro cliente cuando finaliza de operar
        close(connfd);
    }

    // Cierro el socket de escucha cuando el servidor finaliza
    close(sockfd);

    return TODO_OK;
}


// Another version (not working)...
/*
fd_set read_fd;
int32_t server_id, client_id, fdmax, ret, client_no;
struct sockaddr_in server, client;

int main() {
    int32_t opt=1;
    socklen_t len;
    char IP[INET_ADDRSTRLEN],buffer[BUF_LEN];

    if((server_id = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        perror("Unable to create Server ocket");
        exit(-1);
    }

    if( setsockopt(server_id, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {
                perror("setsockopt error...");
                exit(-1);
        }

    memset((char *) &server,0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP , &server.sin_addr);
    bzero(&server.sin_zero,8);

    if((bind(server_id,(struct sockaddr*)&server,sizeof(struct sockaddr_in)))==-1) {
        perror("Unable to bind the server address...");
        exit(-1);
    }

    listen(server_id,5);
    printf("\nWaiting for connection\n");

    FD_ZERO(&read_fd);
    FD_SET(server_id,&read_fd);

    fdmax=server_id;

    while(1){
        FD_SET(server_id,&read_fd);
        printf("%d\n",__LINE__);
        printf("fdmax = %d \n",fdmax);
        if((select(fdmax+1,&read_fd,NULL,NULL,NULL)) <0) {
            perror("Select\n");
            return -1;
            }
        printf("%d\n",__LINE__);

        if(FD_ISSET(server_id,&read_fd)) {
            if((client_id = accept(server_id,(struct sockaddr*)&client,&len)) == -1) {
                perror("Cannot accept the client...");
                exit(-1);
                }
            printf("New client  %d connectd  \n",client_id);
            FD_SET(client_id, &read_fd);
            fdmax = client_id;
            } else {
                for(client_no = server_id+1; client_no <=fdmax; client_no++) {
                    if(FD_ISSET(client_no, &read_fd)) {
                        printf("%d\n",__LINE__);
                        ret = recv(client_no, buffer, BUF_LEN, 0);
                        if(ret == 0) {
                            printf("client  %d disconnectd  \n",client_no);
                            FD_CLR(client_no, &read_fd);
                            close(client_no);
                            break;
                            } else {
                                printf("Received from client %d = %s \n",client_no,buffer);
                            }
                     }
                }
            }

            operar(client_id);
        }
        close(client_id);
        close(server_id);
        return 0;
}
*/
