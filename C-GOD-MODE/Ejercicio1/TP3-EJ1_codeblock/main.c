/**
    Trabajo Práctico 3
    Ejercicio 1
    Entrega

Integrantes del Equipo:

    Franco Dario Scarpello 37842567
    Federico Piacentini 36258738
    Hernan Baini 32883285
    Miguel Amengual 35991055
    Cristian Castagna 37398696
*/

#include "Funciones.h"

int main(int argc, char* argv[])
{
    char opcion;

    if((opcion = getopt(argc, argv, "dhza?")) != -1)
    {
        switch(opcion)
        {
            case 'd':
                opcion_d();
                break;
            case 'h':
                opcion_h();
                break;
            case 'z':
                opcion_z();
                break;
            case 'a':
                mostrar_ayuda();
                break;
            default :
                puts("El parametro enviado es incorrecto. Puede acceder a la ayuda utilizando -a.");
        }
    }
    else
    {
        puts("La llamada es incorrecta. Puede acceder a la ayuda utilizando -a.");
    }

    return 0;
}
