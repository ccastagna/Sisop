#include "functions.h"
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


int main(int argc, char* argv[])
{
    FILE *primerPuntero;
    FILE *segundoPuntero;

    primerPuntero = createFile("TercerArchivo.txt","w+");
    segundoPuntero = createFile("CuartoArchivo.txt","w+");
    fprintf(primerPuntero, "%s %s %s %d", "We", "are", "in", 2012);
    fprintf(segundoPuntero, "%s %s %s %d", "We", "are", "in", 2013);
    fprintf(primerPuntero, "%s %s %s %d", "We", "are", "in", 2014);
    fclose(primerPuntero);
    fclose(segundoPuntero);

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