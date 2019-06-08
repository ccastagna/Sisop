#include <stdio.h>
#include <stdlib.h>

#define DAILY_FILE_NAME "Crear_Multas_AAAAMMDD.txt"
#define TRAFFIC_FILE_NAME "Transito_AAAAMMDD.txt"
#define AMOUNT 15000
#define FALSE 1
#define TRUE 0

double calculateAmount(double mount, double maximumSpeed, double currentSpeed);
int isMaximumSpeedExceded(double maximumSpeed, double currentSpeed);
FILE *createFile(char fileName[], char mode[]); 