Tp3-Ej5-Server : server.o functions.o list.o
	gcc -o Tp3-Ej5-Server server.o functions.o list.o -pthread

server.o : server.c functions.h list.h info.h
	gcc -c server.c
functions.o : functions.c functions.h info.h list.h
	gcc -c functions.c
list.o : list.c info.h list.h
	gcc -c list.c
clean :
	rm *.o
	rm Tp3-Ej5-Server
