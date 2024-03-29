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
#include <signal.h>

/**
    Trabajo Práctico 3
    Ejercicio 3
    Primer Reentrega
Integrantes del Equipo:
    Franco Dario Scarpello 37842567
    Federico Piacentini 36258738
    Hernan Baini 32883285
    Miguel Amengual 35991055
    Cristian Castagna 37398696
*/
// Punteros a los archivos que voy a manejar
FILE *fpToCreateTrafficTicket; // SOLO LOS QUE SUPERAN VEL MAX
FILE *fpToTraffic; // TODOS
char fifoPath[300];
t_cola cola;
// ATRIBUTOS - VAR GLOBALES PARA EL PROCESO DEMONIO
pid_t pid = 0;
pid_t sid = 0;
char *fileName;
// ATRIBUTOS - VAR GLOBALES
int didDayChange = FALSE;
int simulatedEndOfDay = FALSE;
struct tm *fixedDate; // Esta fecha sirve para referencia
struct tm *today; // Esta fecha es la fecha actual

struct DateAndEndOfDay{
    struct tm *fixedDate;
    int isANewDay;
};
struct DateAndEndOfDay *DateAndEndOfDayPointer;
#define OPCION_SALIR 0
#define SI 1
#define NO 0
void imprimirMenu(){
    printf("**************************************************************************\n");
    printf("*                                MENU TP3-EJ3                            *\n");
    printf("**************************************************************************\n");
    printf("1)Descripcion\n");
    printf("2)Ayuda\n");
    printf("3)Ejecutar\n");
    printf("4)Salir\n");
}
int ingresarYValidar(int salida){
    int aDev;
    do{
        printf("Ingrese una Opcion (entre 1 y 4): ");
    scanf("%d",&aDev);
    }while(aDev!=salida && (aDev> 4 ||aDev<1));
    return aDev;
}
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

//FUNCION PARA EL HILO // NO SE USA
void *validateEndOfDay(struct DateAndEndOfDay *pointer) {
    int mustChange = FALSE;
    while(TRUE){
        time_t auxCurrent = time(NULL);
        struct tm currentDate = *localtime(&auxCurrent);
        mustChange = isEndOfTheDay( *(DateAndEndOfDayPointer->fixedDate) , currentDate);
        //Se agrega este if para ver si realmente debo cambiar, ya que sino, solo por un seg estaria en TRUE
        if(mustChange){
            DateAndEndOfDayPointer->isANewDay = TRUE;
        }
        fflush(stdout); // es para imprimir por consola si no ponemos el \n
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
        printf("Para simular fin de dia ejecute: kill %d\n", pid);
        printf("Para detener el servicio, ejecutar: kill -9 %d\n", pid);
        exit(0); //terminate the parent process succesfully
        kill(pid,SIGTERM);
    }

    umask(0);//unmasking the file mode

    sid = setsid();//set new session
    if(sid < 0)
    {
        exit(1);
    }

    //close(STDIN_FILENO);
    //close(STDERR_FILENO);
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

void simularFinDia(){
    printf("Simulacion fin de dia\n");
    simulatedEndOfDay = TRUE;
    // if(fpToCreateTrafficTicket != NULL){
    //     fclose(fpToCreateTrafficTicket);
    // }
    // if(fpToTraffic != NULL){
    //     fclose(fpToTraffic);
    // }
}

t_dato *readFromFifoFile(char *fifoFileName, FILE *fpToTraffic, t_cola *cola){
    t_dato *value =(t_dato*)malloc(sizeof(t_dato));
    if(value==NULL){
        exit(3);
    }

    int fd;
    char readbuf[80];
    int read_bytes;
    fd = open(fifoFileName, O_RDONLY);
    read_bytes = read(fd, readbuf, sizeof(readbuf));
    readbuf[read_bytes] = '\0';

    readString(readbuf, value);
    
    if((*value).speed != 0){
        fprintf(fpToTraffic,"%s %s %d km/h\n", (*value).plate, (*value).camera, (*value).speed);      
        printf("%s %s %d km/h\n", (*value).plate, (*value).camera, (*value).speed);
    }

    if(isMaximumSpeedExceded(MAXIMUM_SPEED, (*value).speed) ){
        time_t auxBillDate = time(NULL);
        struct tm billDate = *localtime(&auxBillDate);
        (*value).hour = billDate.tm_hour;
        (*value).minutes = billDate.tm_min;
        (*value).hour = (*today).tm_hour;
        (*value).minutes = (*today).tm_min;
        acolar(cola,value);
    }
    return value;
}

void *pruebaThread(){
    while(TRUE){
        
        readFromFifoFile(fifoPath, fpToTraffic, &cola);
        fclose(fpToTraffic);
        fpToTraffic = fopen(fileName, "a+");
    }
}

void imprimirDescripcion(){
    printf("Utilice -H o -h o -? para ayuda\n");
    printf("Utilice -D o -d para la descripcion del programa\n");
    printf("Sistema que recibe en un archivo una patente, una camara y una velocidad. \n");
    printf("Se muestra por pantalla dichos datos y de la misma forma se guardan en un \n");
    printf("denominado: “Transito_AAAAMMDD.txt”.\n");
    printf("Al finalizar cada dia, se guarda en un archivo “Crear_Multas_AAAAMMDD.txt”\n");
    printf("la patente de los autos en exceso seguidos de la hora, la velocidad y  el \n");
    printf("importe de la multa. Dicho importe se calcula con la diferencia de veloci \n");
    printf("dad real y la velocidad permitida multiplicado por una constante.\n");
}

void imprimirAyuda(char *nombrePrograma){
    printf("Utilice -H o -h o -? para ayuda\n");
    printf("Utilice -D o -d para la descripcion del programa\n");
    printf("Para ejecutar el programa,  debe ingresar por la opción 3,  y  luego ingresar\n");
    printf("la dirección del archivo FIFO, con el nombre del mismo, entre comillas dobles\n");
    printf("EJ.: \"Desktop/Prueba/ArchivoFifo\" \n");
    printf("Despues apretar ENTER, y desde otra consola, ejecutar un cliente, que escriba\n");
    printf("en el mismo archivo FIFO, con el formato: \n");
    printf("Patente camara velocidad, cada campo separado por un espacio\n");
    printf("Ej.: AAA123 cam5 44\n");
    printf("O bien ejecutar pasando la url del fifo.\n");
    printf("Ej.: %s Desktop/Prueba/ArchivoFifo \n", nombrePrograma);
    printf("Para simular fin de dia ejecute: kill %d\n", pid);
    printf("Para detener el servicio, ejecutar: kill -9 %d\n", pid);

}
int empiezaConGuion(char *array){
    if(array[0] == '-'){
        return TRUE;
    }else{
        return FALSE;
    }
}
int main(int argc, char* argv[]) {

    // Cola para menejar el procesamiento del archivo
    crearCola(&cola);

    // struct sigaction action;
    // memset(&action, 0, sizeof(action));
    // action.sa_handler = term;
    // sigaction(SIGTERM, &action, NULL);

    // Variable local para manejar la logica de apertura de archivo
    int isFirstTime = TRUE;

    // Variable para el manejo de fechas
    time_t auxFixedDate = time(NULL);

    //Logica de fechas
    fixedDate = malloc(sizeof(struct tm));
    *fixedDate = *localtime(&auxFixedDate);
    if(argc == 2){
        if(empiezaConGuion(argv[1])){
            if(!strcmp(argv[1],"-H") || !strcmp(argv[1],"-h") || !strcmp(argv[1],"-?")){
                imprimirAyuda(argv[0]);
                exit(0);
            }else if(!strcmp(argv[1],"-d") || !strcmp(argv[1],"-D")){
                imprimirDescripcion();
                exit(0);
            }else{
                printf("Opcion incorrecta: \n");
                printf("Utilice -H o -h o -? para ayuda\n");
                printf("Utilice -D o -d para la descripcion del programa\n");
                exit(0);
            }
        }else{
            strcpy(fifoPath,argv[1]);
        }
    } else {
        char temp;
        int opcion;
        do
        {
            imprimirMenu();
            opcion = ingresarYValidar(4);
            switch(opcion)
            {
            case 1:
                imprimirDescripcion();
                break;
            case 2:
                imprimirAyuda(argv[0]);
                break;
            case 3:
                printf("Ingrese la ruta del archivo FIFO y a continuacion apriete enter: \n");
                scanf("%c",&temp); // temp statement to clear buffer
                scanf("%[^\n]",fifoPath);
                break;
            case 4:
                exit(0);
                break;
            }
        }while(opcion!=3);
    }
    createDaemonProcess();
    printf("Utilice -H o -h o -? para ayuda\n");
    printf("Utilice -D o -d para la descripcion del programa\n");
    printf("Esperando entradas en: %s \n", fifoPath);
    printf("En una nueva consola, escriba en el archivo anterior: \n");
    printf("Patente Camara Velocidad \n");
    printf("Cada campo separado por un espacio, ej: AAA123 Cam1 120\n");
    //Variables para el FIFO
    /* Create the FIFO if it does not exist */
    mknod(fifoPath, S_IFIFO|0640, 0);
    t_dato dataFromQueue;

    // INICIO DE SERVICIO
    // Variable para el manejo de fechas
    time_t auxCurrent = time(NULL);
    today = malloc(sizeof(struct tm));
    *today = *localtime(&auxCurrent);

    int thread;
    pthread_t threads[NUM_THREADS];
    thread = pthread_create(&threads[0], NULL, pruebaThread, &threads[0]);

    if (signal(SIGTERM, simularFinDia) < 0) {
        printf("\nError al asociar la signal SIGTERM con el handler.");
        return -1;
    }



    while(TRUE) {
        if(isFirstTime) {
            isFirstTime = FALSE;
            fileName = createNameOfFile(TRAFFIC_FILE_NAME,(*fixedDate).tm_mday, (*fixedDate).tm_mon + OFFSET_MONTH, (*fixedDate).tm_year + OFFSET_YEAR);
            if( access(  fileName , F_OK ) != -1 ) {
                fpToTraffic = createFile(fileName,"a+");
            } else {
                fpToTraffic = createFile(fileName,"w+");
            }
        }
        fflush(stdout);
        auxCurrent = time(NULL);
        *today = *localtime(&auxCurrent);
        if(isEndOfTheDay(*fixedDate,(*today)) == TRUE || simulatedEndOfDay == TRUE ){
            simulatedEndOfDay = FALSE;
            fileName = createNameOfFile(DAILY_FILE_NAME,(*fixedDate).tm_mday, (*fixedDate).tm_mon + OFFSET_MONTH, (*fixedDate).tm_year + OFFSET_YEAR);
            fpToCreateTrafficTicket = createFile(fileName,"a+");

            while(colaVacia(&cola) == FALSE){
                desacolar(&cola, &dataFromQueue);
                fprintf(fpToCreateTrafficTicket,"%s %d:%d %d/%d/%d %d km/h $%.0lf\n", dataFromQueue.plate, dataFromQueue.hour, dataFromQueue.minutes, (*fixedDate).tm_mday, (*fixedDate).tm_mon + OFFSET_MONTH, (*fixedDate).tm_year + OFFSET_YEAR  , dataFromQueue.speed, calculateAmount(AMOUNT,MAXIMUM_SPEED,dataFromQueue.speed));
            }
            fclose(fpToCreateTrafficTicket);
            fclose(fpToTraffic);
            auxFixedDate = time(NULL);
            *fixedDate = *localtime(&auxFixedDate); // Actualizo la fecha de referencia
            isFirstTime = TRUE;
        }
    }
    return 0;
}
