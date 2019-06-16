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
#include <fcntl.h>
#include <errno.h>


// typedef struct{
//     char plate[7];
//     char camera[4];
//     int speed;
//     int hour;
//     int minutes;
// }t_dato;

FILE *pf_archLog;

// ATRIBUTOS - VAR GLOBALES PARA EL PROCESO DEMONIO
pid_t pid = 0;
pid_t sid = 0;

// ATRIBUTOS - VAR GLOBALES
int didDayChange = FALSE;
struct tm *fixedDate; // ESTA FECHA SIRVE PARA REFERENCIA

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
        printf("ERROR al abrir el archivo %s", fileName);
        exit(1);
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
            fprintf(pf_archLog,"Cambio de dia \n");
        }
        //fflush(stdout); // es para imprimir por consola si no ponemos el \n
        sleep(1);
    }
}

static void createDaemonProcess(){

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
}

void createThreadDateTime(){
    int thread;
    pthread_t threads[NUM_THREADS];

    thread = pthread_create(&threads[0], NULL, validateEndOfDay, (void *) fixedDate);

    if (thread) {
        printf("Error:unable to create thread, %d\n", thread);
        exit(-1);
    }
    //pthread_exit(NULL); // Cierra el Thread
}

t_dato *readString (char *fifoString, t_dato *value) {
    (*value).plate[0]='\0';
    (*value).camera[0]='\0';
    (*value).speed = 0;;

    if(value == NULL){
        exit(2);
    }
    char delim[] = " ", *p;
    int i;
    for (i = 1, p = strtok(fifoString,delim); p != NULL; p = strtok(NULL,delim), i++) {
        if(i == 1){
            strcpy((*value).plate, p);
        }else if (i == 2){
            strcpy((*value).camera, p);
        }else if (i == 3) {
            (*value).speed = atoi(p);
        }

    }
    return value;
}

t_dato *readFromFifoFile(char *fifoFileName, FILE *fpToTraffic, t_cola *cola){
    t_dato *value =(t_dato*)malloc(sizeof(t_dato));
    if(value==NULL){
        exit(3);
    }

    char end[10];
    strcpy(end, "end"); // TODO Borrar esto;
    int fd;
    char readbuf[80];
    int to_end;
    int read_bytes;
    fd = open(fifoFileName, O_RDONLY);
    read_bytes = read(fd, readbuf, sizeof(readbuf));
    readbuf[read_bytes] = '\0';

    readString(readbuf, value);
    fprintf(fpToTraffic,"%s %s %d km/h\n", (*value).plate, (*value).camera, (*value).speed);
    printf("%s %s %d km/h\n", (*value).plate, (*value).camera, (*value).speed);

    if(isMaximumSpeedExceded(MAXIMUM_SPEED, (*value).speed) ){
        printf("IF");
        time_t auxCurrent = time(NULL);
        struct tm currentDate = *localtime(&auxCurrent);
        (*value).hour = currentDate.tm_hour;
        (*value).minutes = currentDate.tm_min;
        acolar(cola,value);
    }

    to_end = strcmp(readbuf, end);
    if (to_end == 0) {
        fprintf(pf_archLog, "TERMINANDO");
        fclose(fd);
        fclose(pf_archLog);
        exit(TODO_OK);
    }
    didDayChange = TRUE;
    return value;
}

int main(int argc, char* argv[]) {

    // Cola para menejar el procesamiento del archivo
    t_cola cola;
    crearCola(&cola);
    // Archivo de Log
    pf_archLog = createFile("Log.txt","a+");
    fprintf(pf_archLog,"Inicio");
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

    // TODO: Descomentar cuando funcione FIFO
    // createDaemonProcess();
    // createThreadDateTime();

    //Variables para el FIFO

    char fifoFileName[] = "Prueba.txt";

    /* Create the FIFO if it does not exist */
    mknod(fifoFileName, S_IFIFO|0640, 0);
    t_dato dataFromQueue;
    //t_dato *readedFromFifo = NULL;
    // INICIO DE SERVICIO
    while(TRUE) {
        if(isFirstTime) {
            fprintf(pf_archLog,"Iniciando Archivo Trafico Diario");
            isFirstTime = FALSE;
            char *fileName = createNameOfFile(TRAFFIC_FILE_NAME,(*fixedDate).tm_mday, (*fixedDate).tm_mon + OFFSET_MONTH, (*fixedDate).tm_year + OFFSET_YEAR);
            if( access(  fileName , F_OK ) != -1 ) {
                fpToTraffic = createFile(fileName,"a+");
            } else {
                fpToTraffic = createFile(fileName,"w+");
            }
        }


        if(didDayChange){
            didDayChange = FALSE; // Pongo como que ya lo lei, para que no actualice todo
            char *fileName = createNameOfFile(DAILY_FILE_NAME,(*fixedDate).tm_mday, (*fixedDate).tm_mon + OFFSET_MONTH, (*fixedDate).tm_year + OFFSET_YEAR);
            fpToCreateTrafficTicket = createFile(fileName,"w+");
            while(!colaVacia(&cola)){
                desacolar(&cola, &dataFromQueue);
                fprintf(fpToCreateTrafficTicket,"%s %d:%d %d/%d/%d %d km/h $%.0lf\n", dataFromQueue.plate, dataFromQueue.hour, dataFromQueue.minutes, (*fixedDate).tm_mday, (*fixedDate).tm_mon + OFFSET_MONTH, (*fixedDate).tm_year + OFFSET_YEAR  , dataFromQueue.speed, calculateAmount(AMOUNT,MAXIMUM_SPEED,dataFromQueue.speed));
            }
            fclose(fpToCreateTrafficTicket);
            fclose(fpToTraffic);
            *fixedDate = *localtime(&auxFixedDate); // Actualizo la fecha de referencia
            isFirstTime = TRUE;
        }

        readFromFifoFile(fifoFileName, fpToTraffic, &cola);
    }
    // return 0;
}
