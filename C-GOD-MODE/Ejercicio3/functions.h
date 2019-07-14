// Script Tp3-Ej3

// Trabajo practico 3
// Ejercicio 3
// Primera Reentrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "colaDinamica.h"

#define MAXIMUM_SPEED 60
#define NUM_THREADS 1
#define DAILY_FILE_NAME "Crear_Multas"
#define TRAFFIC_FILE_NAME "Transito"
#define AMOUNT 1500
#define FALSE 0
#define TRUE 1

#define TODO_OK 0

#define OFFSET_YEAR 1900
#define OFFSET_MONTH 1


double calculateAmount(double mount, double maximumSpeed, double currentSpeed);
int isMaximumSpeedExceded(double maximumSpeed, double currentSpeed);
FILE *createFile(char fileName[], char mode[]);
char *createNameOfFile(char *string, int day, int month, int year);
int isEndOfTheDay(struct tm fixedDate, struct tm currentDate);
static void createDaemonProcess();
t_dato *readFromFifoFile(char *fifoFileName, FILE *fpToTraffic, t_cola *cola);
