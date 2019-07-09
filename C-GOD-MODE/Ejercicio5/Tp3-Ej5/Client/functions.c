#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> 
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void mostrarMenu(){
    printf("Menu de Opciones: \n");
    printf("1. Ingresar multa.\n");
    printf("2. Mostrar registros a suspender.\n");
    printf("3. Saldar multa.\n");
    printf("4. Buscar monto total de un infractor.\n");
    printf("5. Buscar monto total a pagar de todos los infractores.\n");
    printf("6. Ver menu nuevamente.\n");
    printf("7. Salir.\n");
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
    free(buffer->msg);
    buffer->cantMultas = 0;
    for(int i=0 ; i < MAX_BUFFER_MULTAS; i++){
        buffer->multas[i].cantidad_multas = 0;
        buffer->multas[i].monto_total = 0;
        buffer->multas[i].partido = '\0';
        free(buffer->multas[i].patente);
        free(buffer->multas[i].nombre_titular);
    }
}

int obtenerSemaforo(key_t claveSEM) {
    int idSemaforo;
   
    idSemaforo = semget(claveSEM,1,0600);
    return idSemaforo;
}

void pedirSemaforo(int idSemaforo) {
    printf("pidiendo Semaforo\n");
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
