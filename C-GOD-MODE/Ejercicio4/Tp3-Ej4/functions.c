#include <functions.h>

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

void leerArchivo(FILE **fp, t_list *l){
    char  linea [500],
          *aux;
    t_dato  dato;
    if(!abrir_archivo(&*fp, DATABASE_NAME, READ_TEXT, CON_MSG)) {
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
        str_cpy(&dato.partido, aux + 1);
        *aux = '\0';
        /**patente**/
        aux = strrchr(linea,'|');
        str_cpy(&dato.patente, aux + 1);
        *aux = '\0';
        /**nombre_titular**/
        aux = strrchr(linea,'|');
        str_cpy(&dato.nombre_titular, aux + 1);
        *aux = '\0';
        /**cantidad_multas**/
        aux = strrchr(linea,'|');
        sscanf(linea,"%d",&dato.cantidad_multas);
        *aux = '\0';
        /**monto_total**/
        aux = strrchr(linea,'|');
        sscanf(linea,"%f",&dato.monto_total);
        insertarAlFinal(&l, &dato);
    }
    fclose(*fp);
}

/*
    Recibe patente y el monto de la nueva multa. Si existe suma monto al total y aumenta
    cantidad de multas, sino existe crea un nuevo registro en la base de datos.
*/
int ingresarMulta(char *patente, double monto){
    if (existePatente(&patente)){

    } else {

    }
}

/*
    Recibe la patente y devuelve si existe o no.
*/
int existePatente(char *patente) {

}

/*
    Buscar lista de registros a suspender. Los registros a suspender son aquellos
    de las personas que deben un monto total mayor a $20.000 y/o que poseen más de 3 multas.
    Retorna una lista de ellos.
*/
t_list registrosSuspender(){

}

/*
    Salda la deuda de la patente recibida, es decir lo elimina de la base de datos.
*/
int saldarMulta(char *patente){

}

/*
    Busca el monto total a pagar de la patente recibida.
*/
double buscarMontoTotal(char *patente){

}

/*
    Muestra el monto total a pagar de cada infractor
*/
t_list verMontoTotalInfractores(){

}

int compararPatente (const t_info *a, const t_info *b) {
    if (a->patente == b->patente){
        return 0;
    }
    if (a->patente > b->patente){
        return 1;
    }
    if( a->patente < b->patente){
       return -1;
    }
}
