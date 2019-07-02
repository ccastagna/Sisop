/* Filename: fifoclient.c */
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "FifoFile"

char fifoPath[300];
int main(int argc, char* argv[]) {
	if(argc == 2){
        	strcpy(fifoPath,argv[1]);
	}else{
        	strcpy(fifoPath,"FifoFile");	
	}
   int fd;
   int end_process;
   int stringlen;
   char readbuf[80];
   char end_str[5];
   printf("FIFO_CLIENT: Ingrese Patente Camara Monto\n");
   printf("Ej.: AAA123 Cam1 100\n");
   fd = open(FIFO_FILE, O_CREAT|O_WRONLY);
   strcpy(end_str, "end");
   
   while (1) {
      fgets(readbuf, sizeof(readbuf), stdin);
      stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      end_process = strcmp(readbuf, end_str);
      
      //printf("end_process is %d\n", end_process);
      if (end_process != 0) {
         write(fd, readbuf, strlen(readbuf));
      } else {
         write(fd, readbuf, strlen(readbuf));
         close(fd);
         break;
      }
   }
   return 0;
}