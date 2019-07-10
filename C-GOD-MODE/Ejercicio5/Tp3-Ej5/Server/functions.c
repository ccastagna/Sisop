#include "functions.h"

int abrirArchivo(FILE **fp, const char *nombre, const char *modo, int msj) {
    *fp = fopen(nombre, modo);

    if(*fp){
        return TODO_OK;
    }
    if(msj){
        printf("Error al abrir el archivo \"%s\" en el modo \"%s\".\n",nombre,modo);
    }
    return NOT_OK;
}

int leerArchivo(FILE **fp, t_list *pl){
    char  linea [100],
          *aux;
    linea[0] = '\0';
    t_dato dato;
    if(!abrirArchivo(&*fp, DATABASE_NAME, READ_TEXT, CON_MSG)) {
        return NOT_OK;
    }
    while(fgets(linea, sizeof(linea) ,*fp))
    {
        if ( ( aux = strrchr (linea, '\n') ) == NULL ) {
            fclose(*fp);
            fprintf(stderr,"Error leyendo base de datos.\n");
            return NOT_OK;
        }
        //monto_total
        *aux = '\0';
        aux = strrchr(linea,'|');
        sscanf(aux + 1,"%f", &dato.monto_total);
        *aux = '\0';
        //cantidad_multas
        aux = strrchr(linea,'|');
        sscanf(aux + 1,"%d", &dato.cantidad_multas);
        *aux = '\0';
        //nombre_titular
        aux = strrchr(linea,'|');
        dato.nombre_titular = malloc( sizeof(aux));
        strcpy(dato.nombre_titular, aux + 1);
        *aux = '\0';
        //patente
        aux = strrchr(linea,'|');
        dato.patente = malloc( sizeof(aux));
        strcpy(dato.patente, aux + 1);
        *aux = '\0';
        //partido
        aux = strrchr (linea, '|');
        dato.partido = malloc( sizeof(aux));
        strcpy(dato.partido, linea);

        insertarAlFinal(pl, &dato);
    }
    fclose(*fp);
    return TODO_OK;
}

int escribirArchivo(FILE **fp, t_list *pl){
    if(!abrirArchivo(&*fp, DATABASE_NAME, WRITE_TEXT, CON_MSG)) {
        return NOT_OK;
    }

    t_list *aux = pl;
    t_dato info;
    while(*aux != NULL){
        info = (*aux)->info;
        fprintf(*fp, "%s|%s|%s|%d|%.2f\n", info.partido,
                                           info.patente,
                                           info.nombre_titular,
                                           info.cantidad_multas,
                                           info.monto_total
                                           );
        fflush(stdin);
        aux = &(*aux)->sig;
    }

    fclose(*fp);

    return TODO_OK;
}

/*
    Recibe patente y el monto de la nueva multa. Si existe suma monto al total y aumenta
    cantidad de multas, sino existe crea un nuevo registro en la base de datos.
*/
int ingresarMulta(char *patente, char *partido, float monto, char *nombre_titular , t_list *pl){
    FILE *fp;
    t_dato dato;
    dato.partido = partido;
    dato.patente = patente;
    dato.nombre_titular = nombre_titular;

    if (existePatente(&dato, pl) == TODO_OK){
        buscarYActualizar (pl, &dato, monto, compararPatente);
    } else {

        dato.cantidad_multas = 1;
        dato.monto_total = monto;
        insertarAlFinal(pl, &dato);
    }

    if(escribirArchivo(&fp, pl) == TODO_OK){
        return TODO_OK;
    } else{
        return NOT_OK;
    }

}

/*
    Recibe la patente y devuelve si existe o no.
*/
int existePatente(t_dato *dato, t_list *pl) {
    return buscarEnListaNoOrdenadaPorClave (pl, dato, compararPatente);
}

/*
    Buscar lista de registros a suspender. Los registros a suspender son aquellos
    de las personas que deben un monto total mayor a $20.000 y/o que poseen m�s de 3 multas.
    Retorna una lista de ellos.
*/
int registrosSuspender(t_buffer *buffer, t_list *pl, char *partido){
    t_list *aux = pl;
    t_dato info;

    while(*aux != NULL){
        info = (*aux)->info;
        if (strcmp(info.partido, partido) == 0){
            if (info.monto_total > 20000 ||
                info.cantidad_multas > 3) {

                strcpy(buffer->multas[buffer->cantMultas].patente, info.patente);
                buffer->cantMultas++;

                printf("%s\n", info.patente);
                fflush(stdin);
            }
        }
        aux = &(*aux)->sig;
    }

    return TODO_OK;
}

/*
    Salda la deuda de la patente recibida, es decir lo elimina de la base de datos.
*/
int saldarMulta( char *patente, char *partido, t_list *pl){
    t_dato dato;
    dato.patente = patente;
    dato.partido = partido;
    FILE *fp;

    if (existePatente(&dato, pl) == TODO_OK){
        if(eliminarPorClave(pl, &dato, compararPatente) == TODO_OK){
            if(escribirArchivo(&fp, pl) == TODO_OK){
                    return TODO_OK;
            }
        }
    }

    return NOT_OK;
}

/*
    Busca el monto total a pagar de la patente recibida.
*/
int buscarMontoTotal(t_buffer *buffer, char *patente, char *partido, t_list *pl){
    t_dato dato;
    dato.patente = patente;
    dato.partido = partido;
    if (buscarEnListaNoOrdenadaPorClave (pl, &dato, compararPatente) == TODO_OK){
        strcpy(buffer->multas[0].patente, dato.patente);
        buffer->multas[0].monto_total = dato.monto_total;

        printf("%s\t%.2f\n", dato.patente, dato.monto_total);
        fflush(stdin);
        return TODO_OK;
    }

    return NOT_OK;
}

/*
    Muestra el monto total a pagar de cada infractor
*/
int verMontoTotalInfractores(t_buffer *buffer, t_list *pl, char *partido){
    if (mostrarLista(buffer, pl, partido, compararPartido) == TODO_OK){
        return TODO_OK;
    } else{
        return NOT_OK;
    }
}


int obtenerSemaforo(key_t claveSEM, int valor) {
    int idSemaforo;
    union semun ctlSem;
   
    idSemaforo = semget(claveSEM,1,IPC_CREAT| 0600);
    ctlSem.val = valor;
    semctl(idSemaforo,0,SETVAL,ctlSem);

    return idSemaforo;
}

void pedirSemaforo(int idSemaforo) {
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

void eliminarSemaforo(int idSemaforo) {
    semctl(idSemaforo,0,IPC_RMID);
}
