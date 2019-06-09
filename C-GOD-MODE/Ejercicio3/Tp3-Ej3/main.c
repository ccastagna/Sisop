#include "functions.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


/*
 * Realiza la cuenta de la velocidad maxima con la velocidad de la camara y el monto predefinido
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
 * Genera el nombre del archivo dado los parametros
 */
char *createNameOfFile(char *string, int day, int month, int year){
    char yearBuffer[5];
    char monthBuffer[3];
    char dayBuffer[3];
    snprintf(yearBuffer, 10, "%d", year);
    snprintf(monthBuffer, 10, "%d", month);
    snprintf(dayBuffer, 10, "%d", day);
    //  es el offset del string, AAAA MM DD, el fin del string y el _
    char *result = (char *) malloc(strlen(string) + strlen(yearBuffer) + strlen(monthBuffer) + strlen(dayBuffer) + 2);
    result = strcpy(result, string);
    result = strcat(result, "_");
    result = strcat(result, yearBuffer);
    result = strcat(result, monthBuffer);
    result = strcat(result, dayBuffer);
    result = strcat(result, "\0");
    return result;
}

int main(int argc, char* argv[])
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char* hola = createNameOfFile(DAILY_FILE_NAME, tm.tm_mday, tm.tm_mon + OFFSET_MONTH, tm.tm_year + OFFSET_YEAR);
    printf("%s", hola);

    // pid_t pid = 0;
    // pid_t sid = 0;
    // FILE *fp= NULL;
    // int i = 0;
    // pid = fork();// fork a new child process

    // if (pid < 0)
    // {
    //     printf("fork failed!\n");
    //     exit(1);
    // }

    // if (pid > 0)// its the parent process
    // {
    //    printf("pid of child process %d \n", pid);
    //     exit(0); //terminate the parent process succesfully
    // }

    // umask(0);//unmasking the file mode

    // sid = setsid();//set new session
    // if(sid < 0)
    // {
    //     exit(1);
    // }

    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);

    // fp = fopen ("mydaemonfile.txt", "w+");
    // while (i < 10)
    // {
    //     sleep(1);
    //     fprintf(fp, "%d", i);
    //     i++;
    // }
    // fclose(fp);

    // return (0);
}
