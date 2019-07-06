Tp3-Ej3 : main.o func_ColaDinamica.o 
	gcc -o Tp3-Ej3 main.o func_ColaDinamica.o -pthread

main.o : main.c functions.h
	gcc -c main.c -L pthread
func_ColaDinamica.o : func_ColaDinamica.c colaDinamica.h info.h
	gcc -c func_ColaDinamica.c
clean :
	rm *.o
	rm Tp3-Ej3
