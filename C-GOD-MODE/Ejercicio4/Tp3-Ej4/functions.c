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


/*
    Recibe la patente y devuelve si existe o no.
*/
int existePatente(char *patente) {

}

/*
    Recibe patente y el monto de la nueva multa. Si existe suma monto al total y aumenta
    cantidad de multas, sino existe crea un nuevo registro en la base de datos.
*/
int ingresarMulta(char *patente, double monto){

}


/*
    Buscar lista de registros a suspender. Los registros a suspender son aquellos
    de las personas que deben un monto total mayor a $20.000 y/o que poseen más de 3 multas.
    Retorna una lista de ellos.
*/
t_list registrosSuspender();

/*
    Salda la deuda de la patente recibida, es decir lo elimina de la base de datos.
*/
int saldarMulta(char*);

/*
    Busca el monto total a pagar de la patente recibida.
*/
double buscarMontoTotal(char*);

/*
    Muestra el monto total a pagar de cada infractor
*/
t_list verMontoTotalInfractores();



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
