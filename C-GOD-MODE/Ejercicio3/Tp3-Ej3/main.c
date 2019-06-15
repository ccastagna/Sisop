#include "functions.h"
#include "colaDinamica.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

// ATRIBUTOS - VAR GLOBALES
int didDayChange = FALSE;
struct tm *fixedDate; // ESTA FECHA SIRVE PARA REFERENCIA

// ATRIBUTOS - VAR GLOBALES PARA EL PROCESO DEMONIO
pid_t pid = 0;
pid_t sid = 0;

/*
 * Realiza la cuenta de la velocidad maxima con la velocidad de la camara y el monto predefinido.
 */
double calculateAmount(double mount, double maximumSpeed, double currentSpeed){
    return (currentSpeed - maximumSpeed) * mount;
}

/*
 * Realiza la verificacion de si excede la velocidad o no
 */
int isMaximumSpeedExceded(double maximumSpeed, double currentSpeed){
    return currentSpeed > maximumSpeed ? TRUE : FALSE;
}

/*
 * Crea un archivo indicando el nombre y modo, si falla, retorna NULL.
 */
FILE *createFile(char fileName[], char mode[]){
    FILE *fp;
    fp = fopen (fileName, mode);
    if(fp == NULL){
        return NULL;
    }
    return fp;
}

/*
 * Genera el nombre del archivo dado los parametros.
 */
char *createNameOfFile(char *string, int day, int month, int year){
    char yearBuffer[5];
    char monthBuffer[3];
    char dayBuffer[3];
    snprintf(yearBuffer, 10, "%d", year);
    snprintf(monthBuffer, 10, "%d", month);
    snprintf(dayBuffer, 10, "%d", day);
    //  es el offset del string, AAAA MM DD, el fin del string, el _ y el .txt
    char *result = (char *) malloc(strlen(string) + strlen(yearBuffer) + strlen(monthBuffer) + strlen(dayBuffer) + 6);
    result = strcpy(result, string);
    result = strcat(result, "_");
    result = strcat(result, yearBuffer);
    result = strcat(result, monthBuffer);
    result = strcat(result, dayBuffer);
    result = strcat(result, ".txt");
    result = strcat(result, "\0");
    return result;
}

/*
 * Verifica si cambio de dia.
 */
int isEndOfTheDay(struct tm fixedDate, struct tm currentDate) {
    if(fixedDate.tm_mday == currentDate.tm_mday && fixedDate.tm_mon == currentDate.tm_mon && fixedDate.tm_year == currentDate.tm_year ){
        return FALSE;
    }
    return TRUE;
}

//FUNCION PARA EL HILO
void *validateEndOfDay(void *fixedDate) {
    int mustChange = FALSE;
    while(TRUE){
        struct tm *parameter = (struct tm*)fixedDate;
        time_t auxCurrent = time(NULL);
        struct tm currentDate = *localtime(&auxCurrent);
        mustChange = isEndOfTheDay(*parameter, currentDate);
        //Se agrega este if para ver si realmente debo cambiar, ya que sino, solo por un seg estaria en TRUE
        if(mustChange){
            didDayChange = TRUE;
        }
        //fflush(stdout); // es para imprimir por consola si no ponemos el \n
        sleep(1);
    }
}

void createDaemonProcess(){
    // FILE *fp= NULL;
    // int i = 0;
    pid = fork();// fork a new child process

    if (pid < 0)
    {
        printf("fork failed!\n");
        exit(1);
    }

    if (pid > 0)// its the parent process
    {
       printf("pid of child process %d \n", pid);
        exit(0); //terminate the parent process succesfully
    }

    umask(0);//unmasking the file mode

    sid = setsid();//set new session
    if(sid < 0)
    {
        exit(1);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // fp = fopen ("mydaemonfile.txt", "w+");

    // ACA IMPLEMENTACION DEL SERVICIO
    // while (i < 10)
    // {
    //     sleep(1);
    //     fprintf(fp, "%d", i);
    //     i++;
    // }
    // fclose(fp);

    // return (0);
}

int main(int argc, char* argv[])
{
    // Variables para el manejo de hilos
    pthread_t threads[NUM_THREADS];
    int thread;
    
    // Cola para menejar el procesamiento del archivo
    t_cola cola;
    crearCola(&cola);

    // Punteros a los archivos que voy a manejar
    FILE *fpToTraffic; // TODOS
    FILE *fpToCreateTrafficTicket; // SOLO LOS QUE SUPERAN VEL MAX

    // Variable local para manejar la logica de apertura de archivo
    int isFirstTime = TRUE;

    // Variable para el manejo de fechas
    time_t auxFixedDate = time(NULL);
    
    //Logica de fechas
    fixedDate = malloc(sizeof(struct tm));
    *fixedDate = *localtime(&auxFixedDate);

    //struct tm fixedDate = *localtime(&auxFixedDate);
    thread = pthread_create(&threads[0], NULL, validateEndOfDay, (void *) fixedDate);
    
    if (thread) {
        printf("Error:unable to create thread, %d\n", thread);
        exit(-1);
    }

    //pthread_exit(NULL); // Cierra el Thread

    // INICIO DE SERVICIO
    while(TRUE) {

        if(didDayChange){
            didDayChange = FALSE; // Pongo como que ya lo lei, para que no actualice todo
            *fixedDate = *localtime(&auxFixedDate); // Actualizo la fecha de referencia
            //HACER LA LOGICA DE DESACOLADO Y GENERAR ARCHIVOS.
        }

        // if(isFirstTime) {
        //     isFirstTime = FALSE;
        //     char *fileName = createNameOfFile(TRAFFIC_FILE_NAME,(*fixedDate).tm_mday, (*fixedDate).tm_mon + OFFSET_MONTH, (*fixedDate).tm_year + OFFSET_YEAR);
        //     if( access(  fileName , F_OK ) != -1 ) {
        //         fpToTraffic = createFile(fileName,"a+");
        //     } else {
        //         fpToTraffic = createFile(fileName,"w+");
        //     }
        // }
        // fclose(fpToTraffic);
    }

    
}
