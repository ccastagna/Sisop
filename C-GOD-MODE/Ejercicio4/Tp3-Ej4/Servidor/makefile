Tp3-Ej4-Servidor : main.o functions.o list.o
	gcc -o Tp3-Ej4-Servidor main.o functions.o list.o -pthread

main.o : main.c functions.h list.h info.h
	gcc -c main.c -L pthread
functions.o : functions.c functions.h info.h list.h
	gcc -c functions.c
list.o : list.c info.h list.h
	gcc -c list.c
clean :
	rm *.o
	rm Tp3-Ej4-Servidor
