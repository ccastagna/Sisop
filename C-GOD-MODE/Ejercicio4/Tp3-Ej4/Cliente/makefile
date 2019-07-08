Tp3-Ej4-Cliente : main.o functions.o
	gcc -o Tp3-Ej4-Cliente main.o functions.o

main.o : main.c functions.h info.h
	gcc -c main.c
functions.o : functions.c functions.h info.h
	gcc -c functions.c
clean :
	rm *.o
	rm Tp3-Ej4-Cliente
