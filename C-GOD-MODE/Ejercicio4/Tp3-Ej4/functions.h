#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define NO_EXISTE_PATENTE 0
#define EXISTE_PATENTE 1

#define TODO_OK 0
#define NOT_OK 1

// Usado para controlar que registros debo eliminar de la base de datos.
#define NO_CAMBIO 0
#define CAMBIO 1
#define ELIMINAR 2

#define DATABASE_NAME "infractores.txt"

int abrirArchivo(FILE **, const char*, const char*, int);

//leerArchivo();

//escribirArchivo();

/*
    Recibe patente y el monto de la nueva multa. Si existe suma monto al total y aumenta
    cantidad de multas, sino existe crea un nuevo registro en la base de datos.
*/
int ingresarMulta(char*, double);

/*
    Recibe la patente y devuelve si existe o no.
*/
int existePatente(char*);

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

#endif // FUNCTIONS_H_INCLUDED
