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


void limpiarBuffer(struct t_buffer *buffer){
    buffer->opcion = 0;
    buffer->msg = "\0";
    buffer->cantMultas = 0;
    for(int i=0 ; i < multas.length ; i++){
        buffer->multas[i].cantidad_multas = 0;
        buffer->multas[i].monto_total = 0;
        buffer->multas[i].nombre_titular = '\0';
        buffer->multas[i].partido = '\0';
        buffer->multas[i].patente = '\0';
    }
}