Tp3-Ej5-Client : client.o functions.o
	gcc -o Tp3-Ej5-Client client.o functions.o -pthread -lrt

client.o : client.c functions.h 
	gcc -c client.c  -pthread -lrt
functions.o : functions.c functions.h ../Server/info.h 
	gcc -c functions.c
clean :
	rm *.o
	rm Tp3-Ej5-Client
