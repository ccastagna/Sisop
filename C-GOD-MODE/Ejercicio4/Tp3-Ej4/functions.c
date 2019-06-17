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

int leerArchivo(FILE **fp, t_list *pl, char *partido){
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

        if (strcmp(dato.partido, partido) == 0){
            insertarAlFinal(pl, &dato);
        }

    }
    fclose(*fp);
    return TODO_OK;
}

/*
    Recibe patente y el monto de la nueva multa. Si existe suma monto al total y aumenta
    cantidad de multas, sino existe crea un nuevo registro en la base de datos.
*/
int ingresarMulta(char *patente, double monto, t_list *pl){
    if (existePatente(patente, &pl)){

    } else {

    }

    return TODO_OK;
}

/*
    Recibe la patente y devuelve si existe o no.
*/
int existePatente(char *patente, t_list *pl) {

    return TODO_OK;
}

/*
    Buscar lista de registros a suspender. Los registros a suspender son aquellos
    de las personas que deben un monto total mayor a $20.000 y/o que poseen más de 3 multas.
    Retorna una lista de ellos.
*/
int registrosSuspender(t_list *pl){

    return TODO_OK;
}

/*
    Salda la deuda de la patente recibida, es decir lo elimina de la base de datos.
*/
int saldarMulta(char *patente, t_list *pl){

    return TODO_OK;
}

/*
    Busca el monto total a pagar de la patente recibida.
*/
double buscarMontoTotal(char *patente, t_list *pl){

    return TODO_OK;
}

/*
    Muestra el monto total a pagar de cada infractor
*/
int verMontoTotalInfractores(t_list *pl, char *partido){
    if (mostrarLista(pl, partido, compararPartido) == TODO_OK){
        return TODO_OK;
    } else{
        return NOT_OK;
    }
}
