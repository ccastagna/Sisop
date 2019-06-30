#include "functions.h"

void mostrarMenu(int sockfd){
    char buff[] = "Menu de Opciones: \n \
                    1. Ingresar multa.\n \
                    2. Mostrar registros a suspender.\n \
                    3. Saldar multa.\n \
                    4. Buscar monto total de un infractor.\n \
                    5. Buscar monto total a pagar de todos los infractores.\n \
                    6. Ver menu nuevamente.\n \
                    7. Salir.\n\0";
    write(sockfd, buff, sizeof(buff));
}

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

/*
    Recibe patente y el monto de la nueva multa. Si existe suma monto al total y aumenta
    cantidad de multas, sino existe crea un nuevo registro en la base de datos.
*/
int ingresarMulta(const char *patente, const char *partido, const char *nombre_titular, const float monto, t_list *pl){
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
int existePatente(const t_dato *dato, t_list *pl) {
    return buscarEnListaNoOrdenadaPorClave (pl, dato, compararPatente);
}

/*
    Buscar lista de registros a suspender. Los registros a suspender son aquellos
    de las personas que deben un monto total mayor a $20.000 y/o que poseen más de 3 multas.
    Retorna una lista de ellos.
*/
int registrosSuspender(t_list *pl, const char *partido, int sockfd){
    t_list *aux = pl;
    t_dato info;

    char buff[10];

    while(*aux != NULL){
        info = (*aux)->info;
        if (strcmp(info.partido, partido) == 0){
            if (info.monto_total > 20000 ||
                info.cantidad_multas > 3) {

                bzero(buff, sizeof(buff));
                strcpy(buff, info.patente);
                write(sockfd, buff, sizeof(buff));
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
int saldarMulta(const char *patente, const char *partido, t_list *pl){
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
int buscarMontoTotal(const char *patente, const char *partido, t_list *pl, int sockfd){
    t_dato dato;
    dato.patente = patente;
    dato.partido = partido;

    char buff[30];
    char *aux = malloc(23);
    if (buscarEnListaNoOrdenadaPorClave (pl, &dato, compararPatente) == TODO_OK){
        bzero(buff, sizeof(buff));
        strcpy(buff, dato.patente);
        strcat(buff, "\t");
        sprintf(aux, "%f", dato.monto_total);
        strcat(buff, aux);
        write(sockfd, buff, sizeof(buff));

        return TODO_OK;
    }
    free(aux);

    return NOT_OK;
}

/*
    Muestra el monto total a pagar de cada infractor
*/
int verMontoTotalInfractores(t_list *pl, const char *partido, int sockfd){
    if (mostrarLista(pl, partido, compararPartido, sockfd) == TODO_OK){
        return TODO_OK;
    } else{
        return NOT_OK;
    }
}
