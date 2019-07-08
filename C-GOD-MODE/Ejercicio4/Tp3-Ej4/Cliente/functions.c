#include "functions.h"

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


