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

#include "functions.h"

/*
 * Muestra la ayuda para el cliente.
 */
void mostrarAyuda(char *nombre) {
   printf("\nSistema utilizado para conectarse al servidor central de infracciones de La Plata.\n\n");
   printf("Para iniciar el cliente ejecute lo siguiente:\n");
   printf("\t%s <ip_servidor> <puerto_servidor> <partido>\n\n", nombre);
   printf("Parametros:\n");
   printf("\t<ip_servidor>: IP o nombre del equipo del servidor al que se desea conectar.\n");
   printf("\t<puerto_servidor>: puerto del servidor al que se desea conectar.\n");
   printf("\t<partido>: partido desde el que se desea operar.\n\n");
   printf("Ejemplo:\n");
   printf("\t%s localhost 8181 Lanus\n\n", nombre);
}


/*
 * Funcion utilizada para escribir mensajes al cliente.
 * Primero se le envia una cabecera indicandole que tipo de mensaje le sera enviado.
 * Luego le envia el cuerpo del mensaje al cliente.
 */
void escribirMensaje (int socket, int idMensaje, char *mensaje, int tamanho) {
    // Se declara y rellena la cabecera
    t_cabecera cabecera;
    cabecera.identificador = idMensaje;
    cabecera.longitud = tamanho;

    // Se envía la cabecera
    write (socket, &cabecera, sizeof(cabecera));

    // Si el mensaje no tiene cuerpo, hemos terminado
    if ((mensaje == NULL) || (tamanho == 0)){
            printf("No hay mensaje a enviar a cliente %d...", socket);
    }

    // Se envía el cuerpo
    write (socket, mensaje, tamanho);
}

/*
 * Funcion utilizada para leer los mensajes recibidos.
 */
void leerMensaje (int socket, int *id_mensaje,  char *mensaje) {
    t_cabecera cabecera;
    read (socket, &cabecera, sizeof(cabecera)); // Se lee la cabecera enviada

    // Rellenamos el identificador para devolverlo
    *id_mensaje = cabecera.identificador;

    // Si hay que leer una estructura detrás
    if (cabecera.longitud > 0) {
        while(read(socket, mensaje, cabecera.longitud) < cabecera.longitud - 1);
    }
}


