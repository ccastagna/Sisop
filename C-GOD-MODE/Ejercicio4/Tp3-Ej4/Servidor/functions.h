#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "info.h"
#include "list.h"

#define NO_EXISTE_PATENTE   0
#define EXISTE_PATENTE      1

// Usado para archivos
#define DATABASE_NAME	    "infractores" // nombre de la base de datos.
#define READ_TEXT           "rt"    //Abre un fichero de texto para lectura.
#define READ_TEXT_UPDATE    "r+t"   //Abre un fichero de texto para actualización (lectura y escritura).
#define WRITE_TEXT          "wt"    //Crea o pisa el contenido de un archivo.
#define CON_MSG             1

/*
 * Funcion para mostrar la ayuda del servidor.
 */
void mostrarAyuda(char *);

/*
 * Recibe un buffer donde guardar el menu.
 */
void mostrarMenu(char *);

/*
 * Recibe puntero a archivo, el nombre del archivo, el modo,
 * y si usa o no mensaje
 */
int abrirArchivo(FILE **, const char*, const char*, int);

/*
 * Recibe el puntero a archivo y puntero a lista.
 * Lee el archivo y lo pasa a la lista, que sera compartida para todos los clientes.
 */
int leerArchivo(FILE **, t_list *);

/*
 * Recibe el puntero a archivo y puntero a lista.
 * Sobreescribe el archivo con la información que hay en la lista.
 */
int escribirArchivo(FILE **, t_list *);

/*
 * Recibe la cadena a normalizar y el tamaño.
 * Elimina espacios de mas y pasa todo a minusculas.
 */
int normalizarCadena(char *, int);

/*
    Recibe patente y el monto de la nueva multa. Si existe suma monto al total y aumenta
    cantidad de multas, sino existe crea un nuevo registro en la base de datos.
*/
int ingresarMulta(char*, char*, char*, const float, t_list *);

/*
    Recibe la patente y devuelve si existe o no.
*/
int existePatente(t_dato*, t_list *);

/*
    Buscar lista de registros a suspender. Los registros a suspender son aquellos
    de las personas que deben un monto total mayor a $20.000 y/o que poseen más de 3 multas.
    Retorna una lista de ellos.
*/
int registrosSuspender(t_list *, char*, char*);

/*
    Salda la deuda de la patente recibida, es decir lo elimina de la base de datos.
*/
int saldarMulta(char*, char*, t_list *);

/*
    Busca el monto total a pagar de la patente recibida.
*/
int buscarMontoTotal(char*, char*, t_list*, char*);

/*
    Muestra el monto total a pagar de cada infractor.
*/
int verMontoTotalInfractores(t_list*, char*, char*);

/*
 * Funcion utilizada para escribir mensajes al cliente.
 * Primero se le envia una cabecera indicandole que tipo de mensaje le sera enviado.
 * Luego le envia el cuerpo del mensaje al cliente.
 */
void escribirMensaje (int, int, char *, int);

/*
 * Funcion utilizada para leer los mensajes recibidos.
 */
void leerMensaje (int , int *, char *);


#endif // FUNCTIONS_H_INCLUDED
