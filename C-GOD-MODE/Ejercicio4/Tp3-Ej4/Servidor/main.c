// Script Tp3-Ej4-Servidor

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
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

// Bibliotecas propias.
#include "info.h"
#include "list.h"
#include "functions.h"

#define MAX                         1024
#define SA                          struct sockaddr
#define MAX_PENDING_CONNECTIONS     5

// Lista compartida por todos los clientes.
t_list lista;

// Semaforo mutex para recursos compartidos.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Contador de clientes actuales.
int client_counter;

// Mensajes a clientes.
char welcome[] = "Bienvenido al sistema de infracciones de transito del gobierno.\n\0";
char indicate_option[] = "\nIndique la opcion: \0";
char invalid_option[] = "\nOpcion invalida, ingrese un numero entre 1 y 7: \0";
char ask_patente[] = "Ingrese patente: \0";
char ask_amount[] = "Ingrese monto de la multa: \0";
char ask_titular_name[] = "Ingrese nombre del titular: \0";
char fee_succesfull[] = "Se ingreso la multa exitosamente.\n\0";
char fee_unsuccesfull[] = "La multa no se pudo generar, intente nuevamente.\n\0";
char paid_pending_fee[] = "Se saldaron las multas de la patentes %s";
char not_pending_fee[] = "La patente ingresada no tiene multas a saldar.\n\0";
char common_not_found_message[] = "No se encontraron infractores.\n\0";

// Declaracion de prototipos de funcion.
void INThandler(int);
void *operar(void *);

// logica principal.
int main(int argc, char *argv[]) {
    // Puntero a archivo de la BD.
    FILE *fp;

    // Declarcion de variables.
    int serverSocket, newSocket, portno, max_client;
    struct sockaddr_in servaddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // Controlar senhales enviadas como Ctrl+C.
    signal(SIGINT, INThandler);

    if(argc == 2) {
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-H") || !strcmp(argv[1], "-?")) {
                mostrarAyuda(argv[0]);
                exit((int)TODO_OK);
        }
    }
    
    if(argc < 3) {
	fprintf(stderr,"\nIngrese %s -h para obtener ayuda.\n\n", argv[0]);
        exit((int)TODO_OK);
    }
 
    // Mapeo a variables los parametros recibidos.
    portno = atoi(argv[1]);
    max_client = atoi(argv[2]);

    // Creo la lista que estará compartida para todos los sockets.
    crearLista(&lista);
    leerArchivo(&fp, &lista);

    // Creacion y verificacion de socket.
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        printf("Socket creation failed...\n");
        exit((int)NOT_OK);
    }
    else {
        printf("Socket successfully created...\n");
    }
    memset(&servaddr, 0, sizeof(servaddr));

    // Asigno ip y puerto.
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portno);

    // Binding newly created socket to given IP and verification
    if ((bind(serverSocket, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit((int)NOT_OK);
    }
    else{
        printf("Socket successfully binded..\n");
    }

    // Server listo para escuchar y verificacion
    if ((listen(serverSocket, MAX_PENDING_CONNECTIONS)) != 0) {
        printf("Listen failed...\n");
        exit((int)NOT_OK);
    }
    else {
        printf("Server listening on port %d..\n", portno);
    }

    pthread_t tid;

    client_counter = 0;

    while(1) {

	if(client_counter < max_client) {
		// Aceptar llamada de creacion de nuevo socket por conexion entrante.
        	addr_size = sizeof(serverStorage);
	        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

		// Por cada peticion de nuevo cliente creo un thread y asigno la peticion del cliente a su procesamiento.
	        // El thread pricipal puede atender a la proxima solicitud
        	if( pthread_create(&tid, NULL, operar, &newSocket) != 0 ){
		   printf("Failed to create thread...\n");
		}

	client_counter ++;
	}
	   
    }

    // Cierro el socket de escucha cuando el servidor finaliza
    close(serverSocket);

    return (int)TODO_OK;
}


void INThandler(int sig) {
     char  c;

     printf("\nRealmente desea cerrar el servidor? [S/N]: ");
     c = getchar();
     if (c == 's' || c == 'S') {
          exit(0);
     } else {
          signal(SIGINT, INThandler);
     }
     getchar();
}


void *operar(void *arg) {
    int sockfd = *((int *)arg);
    // opcion del menu
    int code;
  
    //variables para operar
    char *partido = malloc(25);
    *partido = '\0';
    char *patente = malloc(8);
    *patente = '\0';
    char *nombre_titular = malloc(25);
    *nombre_titular = '\0';
    float monto;

    // buffer usado para dar las respuestas al cliente
    char buff[MAX];

    // Identificador de cabecera
    int identificador;

    // Muestro mensaje de bienvenida
    escribirMensaje(sockfd, id_welcome, welcome, strlen(welcome));

    // Leo partido del cliente
    memset(partido, 0, strlen(partido));
    leerMensaje(sockfd, &identificador, partido); 
    
    // Normalizo nombre del partido
    normalizarCadena(partido, strlen(partido));
    printf("El cliente %d es del partido %s.\n", sockfd, partido);

    
    // Le envio menu al cliente
    memset(buff, 0, MAX);
    mostrarMenu(buff);
    escribirMensaje(sockfd, id_menu, buff, strlen(buff));
    
    while (1) {
        sleep(1);
        // Le pido a cliente que ingrese una opcion
        memset(buff, 0, MAX);
	escribirMensaje(sockfd, id_indicate_option, indicate_option, sizeof(indicate_option));
        fflush(stdout);

        // Leo opcion ingresada por el cliente
        memset(buff, 0, MAX);
        leerMensaje(sockfd, &identificador, buff);
        code = atoi(buff);

        // Si el codigo es 0 es porque el cliente salio de la consola con una señal no tenida en cuenta.
        // Asigno codigo 7 para entrar en flujo de cierre de conexion.
        if (code == 0){
            code = 7;
        }

        printf("El cliente %d selecciono la opcion %d.\n", sockfd, code);

        fflush(stdout);

        switch (code) {
            case 1: // Ingresar multa nueva.
                // Le pido al usuario que ingrese la patente.
		escribirMensaje(sockfd, id_ask_patente, ask_patente, strlen(ask_patente));
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                strcpy(patente, buff);

		printf("El cliente %d ingreso la patente %s.\n", sockfd, patente);

                // Le pido al usuario que ingrese el monto de la multa.
                escribirMensaje(sockfd, id_ask_amount, ask_amount, strlen(ask_amount));
                
		// Leo el monto que me envia el usuario.
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                monto = atof(buff);

		printf("El cliente %d ingreso el monto %.2f.\n", sockfd, monto);

                // Le pido al usuario que ingrese el nombre del titular.
		escribirMensaje(sockfd, id_ask_titular_name, ask_titular_name, strlen(ask_titular_name));
                
		// Leo el nombre del titular.
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                strcpy(nombre_titular, buff);

		printf("El cliente %d ingreso el nombre titular %s.\n", sockfd, nombre_titular);
		
		if(strlen(patente) > 2 && strlen(nombre_titular) > 2 && monto > 0) {
			pthread_mutex_lock(&mutex);
                	if (ingresarMulta(patente, partido, nombre_titular, monto, &lista) == TODO_OK){
				escribirMensaje(sockfd, id_fee_succesfull, fee_succesfull, strlen(fee_succesfull));
	                } else {
				escribirMensaje(sockfd, id_fee_unsuccesfull, fee_unsuccesfull, strlen(fee_unsuccesfull));
                	}
			pthread_mutex_unlock(&mutex);
		} else {
			escribirMensaje(sockfd, id_fee_unsuccesfull, fee_unsuccesfull, strlen(fee_unsuccesfull));
                }

                fflush(stdout);
                break;
            case 2: // Obtener registros a suspender.
                memset(buff, 0, MAX);

		pthread_mutex_lock(&mutex);
                if (registrosSuspender(&lista ,partido, buff) == TODO_OK) {
		   escribirMensaje(sockfd, id_registros_suspender, buff, strlen(buff));
                } else{
		    escribirMensaje(sockfd, id_common_not_found_message, common_not_found_message, sizeof(common_not_found_message));
                }
		pthread_mutex_unlock(&mutex);

                fflush(stdout);
                break;
            case 3: // Saldar multa.
		// Pido patente al cliente.
		escribirMensaje(sockfd, id_ask_patente, ask_patente, strlen(ask_patente));

		// Leo patente.
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                strcpy(patente, buff);
		printf("El cliente %d busca infractor con patente %s.\n", sockfd, patente);
		
		pthread_mutex_lock(&mutex);
                if (saldarMulta(patente, partido, &lista) == TODO_OK){
		    	escribirMensaje(sockfd, id_paid_pending_fee, paid_pending_fee, strlen(paid_pending_fee));
                } else {
			escribirMensaje(sockfd, id_not_pending_fee, not_pending_fee, strlen(not_pending_fee));
		}
		pthread_mutex_unlock(&mutex);
                fflush(stdout);
                break;
            case 4: // Obtener datos de un infractor.
                // Le pido al cliente que ingrese patente.
		escribirMensaje(sockfd, id_ask_patente, ask_patente, strlen(ask_patente));

                // Leo la patente enviada por el cliente.
                memset(buff, 0, MAX);
                leerMensaje(sockfd, &identificador, buff);
                strcpy(patente, buff);

                memset(buff, 0, MAX);
                printf("El cliente %d busca infractor con patente %s.\n", sockfd, patente);

		pthread_mutex_lock(&mutex);
                // Le indico al usuario lo que debe la patente o le indico que no existe.
                if (buscarMontoTotal(patente, partido, &lista, buff) == TODO_OK) {
         		escribirMensaje(sockfd, id_buscar_infractor, buff, strlen(buff));
                } else {
			escribirMensaje(sockfd, id_common_not_found_message, common_not_found_message, strlen(common_not_found_message));
                }
		pthread_mutex_unlock(&mutex);

                fflush(stdout);
                break;
            case 5: // Obtener todos los infractores de un partido.
                memset(buff, 0, MAX);

		pthread_mutex_lock(&mutex);
                // Si hay infractores para el partido los muestro, sino indico que no hay.
		if(verMontoTotalInfractores (&lista, partido, buff) == TODO_OK) {
			escribirMensaje(sockfd, id_monto_total_infractores, buff, strlen(buff));
                } else{
			escribirMensaje(sockfd, id_common_not_found_message, common_not_found_message, strlen(common_not_found_message));
                }
		pthread_mutex_unlock(&mutex);

                fflush(stdout);
                break;
            case 6: // Mostrar menu.
		memset(buff, 0, MAX);
                mostrarMenu(buff);
		escribirMensaje(sockfd, id_menu, buff, strlen(buff));
                fflush(stdout);
                break;
            case 7: // Salida de cliente.
                printf("El cliente %d cierra la sesion exitosamente.\n", sockfd);
                client_counter--;
		close(sockfd);
		pthread_exit(NULL);
                break;
            default: // Salida de cliente.
                printf("Se cierra la sesion del cliente %d debido a un problema.\n", sockfd);
		client_counter--;
		close(sockfd);
		pthread_exit(NULL);
        }
    }
}
