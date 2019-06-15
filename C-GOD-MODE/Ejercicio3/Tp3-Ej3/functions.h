#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 1
#define DAILY_FILE_NAME "Crear_Multas"
#define TRAFFIC_FILE_NAME "Transito"
#define AMOUNT 15000
#define FALSE 0
#define TRUE 1

#define OFFSET_YEAR 1900
#define OFFSET_MONTH 1


double calculateAmount(double mount, double maximumSpeed, double currentSpeed);
int isMaximumSpeedExceded(double maximumSpeed, double currentSpeed);
FILE *createFile(char fileName[], char mode[]);
char *createNameOfFile(char *string, int day, int month, int year);
int isEndOfTheDay(struct tm fixedDate, struct tm currentDate);
static void createDaemonProcess();
void createThreadDateTime();
