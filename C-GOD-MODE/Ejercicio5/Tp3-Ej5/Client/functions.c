// Script Tp3-Ej5-Client

// Trabajo Practico 3
// Ejercicio 5
// Primera Reentrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> 
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void mostrarMenu(){
    printf("\n\n\nMenu de Opciones: \n");
    printf("1. Ingresar multa.\n");
    printf("2. Mostrar registros a suspender.\n");
    printf("3. Saldar multa.\n");
    printf("4. Buscar monto total de un infractor.\n");
    printf("5. Buscar monto total a pagar de todos los infractores.\n");
    printf("6. Salir.\n");
    fflush(stdin);
}

int normalizarCadena(unsigned char *buf, int len) {
    int i, j;

    for(j=i=0 ;i < len; ++i){
        if(isupper(buf[i])) {
            buf[j++]=tolower(buf[i]);
            continue ;
        }
        if(isspace(buf[i])){
            if(!j || j && buf[j-1] != ' ')
                buf[j++]=' ';
            continue ;
        }
        buf[j++] = buf[i];
    }
    buf[j+1] = '\0';

    return j;
}


void limpiarBuffer( t_buffer *buffer){
    buffer->opcion = 0;
    strcpy(buffer->msg, "\0");
    buffer->cantMultas = 0;
    for(int i=0 ; i < MAX_BUFFER_MULTAS; i++){
        buffer->multas[i].cantidad_multas = 0;
        buffer->multas[i].monto_total = 0;
        strcpy(buffer->multas[i].partido, "\0");
        strcpy(buffer->multas[i].patente, "\0");
        strcpy(buffer->multas[i].nombre_titular, "\0");
    }
}

int obtenerSemaforo(key_t claveSEM) {
    int idSemaforo;
   
    idSemaforo = semget(claveSEM,1,0600);
    return idSemaforo;
}

void pedirSemaforo(int idSemaforo) {
    //printf("pidiendo Semaforo\n");
    struct sembuf opSem;
    opSem.sem_num = 0;
    opSem.sem_op = -1;
    opSem.sem_flg = 0;
    semop(idSemaforo,&opSem,1);
}

void devolverSemaforo(int idSemaforo) {
    struct sembuf opSem;
    opSem.sem_num = 0;
    opSem.sem_op = 1;
    opSem.sem_flg = 0;
    semop(idSemaforo,&opSem,1);
}

void mostrarAyuda(char *nombre) {
   printf("\nSistema utilizado para conectarse al servidor central de infracciones de La Plata.\n\n");
   printf("Para iniciar el cliente ejecute lo siguiente:\n");
   printf("\t%s\n\n", nombre);
}
