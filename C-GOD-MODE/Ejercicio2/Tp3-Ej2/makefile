Tp3-Ej2 : main.o colaDinamica.o functions.o
	gcc -o Tp3-Ej2 main.o colaDinamica.o functions.o -pthread

main.o : main.c functions.h colaDinamica.h
	gcc -c main.c -L pthread
functions.o : functions.c functions.h colaDinamica.h
	gcc -c functions.c
colaDinamica.o : colaDinamica.c info.h colaDinamica.h
	gcc -c colaDinamica.c
clean :
	rm *.o
	rm Tp3-Ej2
