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
    char  linea [500],
          *aux;
    t_dato  dato;
    if(!abrirArchivo(&*fp, DATABASE_NAME, READ_TEXT, CON_MSG)) {
        return NOT_OK;
    }
    while(fgets(linea, sizeof(linea) ,*fp))
    {
        if ( ( aux = strrchr (linea, '\n') ) == NULL )
        {
            fclose(*fp);
            fprintf(stderr,"Error leyendo base de datos.\n");
            return NOT_OK;
        }
        /**partido**/
        *aux = '\0';
        aux = strrchr (linea, '|');
        strcpy(&dato.partido, aux + 1);
        *aux = '\0';
        if (strcmp(&dato.partido, *partido) == 0){
            /**patente**/
            aux = strrchr(linea,'|');
            strcpy(&dato.patente, aux + 1);
            *aux = '\0';
            /**nombre_titular**/
            aux = strrchr(linea,'|');
            strcpy(&dato.nombre_titular, aux + 1);
            *aux = '\0';
            /**cantidad_multas**/
            aux = strrchr(linea,'|');
            sscanf(linea,"%d",&dato.cantidad_multas);
            *aux = '\0';
            /**monto_total**/
            aux = strrchr(linea,'|');
            sscanf(linea,"%lf",&dato.monto_total);
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
int verMontoTotalInfractores(t_list *pl){

    return TODO_OK;
}
