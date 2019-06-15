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


typedef struct{
    char plate[7];
    char camera[4];
    int speed;
}t_fifo;

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

t_fifo *readString (char *fifoString){

    t_fifo *valueToReturn=(t_fifo*)malloc(sizeof(t_fifo));
    char delim[] = " ";
    char *p;
    int i;
    char plate = "ASD123";
    for (i = 1, p = strtok(fifoString,delim); p != NULL; p = strtok(NULL,delim), i++) {
        printf("Output%u=%s;\n", i, p);
        if(i == 1){
            strcpy((*valueToReturn).plate, p);
        }else if (i == 2){
            strcpy((*valueToReturn).camera, p);
        }else if (i == 3) {
            (*valueToReturn).speed = atoi(p);
        }

    }
    // while( token != NULL ) {
    //     counter++;
    //     token = strtok(fifoString, delim);
    //     token = strtok(NULL, delim);

    //     if(counter == 1){
    //         strcpy(*valueToReturn->plate, token);
    //     }else if (counter == 2){
    //         strcpy(*valueToReturn->camera, token);
    //     }else {
    //         valueToReturn->speed = atoi(token);
    //     }
    //     printf(token);
    // }
     return valueToReturn;
}

int main(int argc, char* argv[]) {

    // Cola para menejar el procesamiento del archivo
    t_cola cola;
    crearCola(&cola);
    // Archivo de Log
    pf_archLog = createFile("Log.txt","w+");
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

    //VAriables para el FIFO
    int fd;
    char readbuf[80];
    char end[10];
    int to_end;
    int read_bytes;
    char fifoFileName[] = "Prueba.txt";
    /* Create the FIFO if it does not exist */
    mknod(fifoFileName, S_IFIFO|0640, 0);
    strcpy(end, "end");
    // INICIO DE SERVICIO
    while(TRUE) {
        // if(didDayChange){
        //     didDayChange = FALSE; // Pongo como que ya lo lei, para que no actualice todo
        //     *fixedDate = *localtime(&auxFixedDate); // Actualizo la fecha de referencia
        //     //HACER LA LOGICA DE DESACOLADO Y GENERAR ARCHIVOS.
        // }

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


        // LOGICA FIFO
        printf("----->");
        fd = open(fifoFileName, O_RDONLY);
        read_bytes = read(fd, readbuf, sizeof(readbuf));
        readbuf[read_bytes] = '\0';
        printf("Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
        fprintf(pf_archLog,"Se Recibe: \"%s\" \n",readbuf );
        readString(readbuf);
        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
            fprintf(pf_archLog, "TERMINANDO");
            close(fd);
            close(pf_archLog);
            break;
        }
    }


    // return 0;
}
