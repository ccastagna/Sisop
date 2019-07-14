// Script Tp3-Ej1

// Trabajo práctico 3
// Ejercicio 1
// Primera Reentrega

// Integrantes del Equipo
// Franco Dario Scarpello 37842567
// Federico Piacentini 36258738
// Hernan Baini 32883285
// Miguel Amengual 35991055
// Cristian Castagna 3739869

#include "Funciones.h"

void procesamiento()
{
    double variable;

    while(1!=2)
    {
        sleep(2);
        variable = 0;
        for(int i = 0; i<1000000; i++)
        {
            variable++;
        }
    }
}

/** -d cree dos procesos demonios, que deberán simplemente ejecutar cada 2 segundos un
loop hasta 1.000.000 realizando una acumulación en una variable, y no finalizan
(simplemente para que generen algo de procesamiento). */
void opcion_d()
{
    int padre = getpid(),
        hijo1,
        hijo2;

    printf("PID: %d - Proceso Padre.\n", padre);

    if((hijo1 = fork()) == 0)
    {
        waitpid(hijo1,NULL,0);
        printf("PID: %d PPID: %d - Parentesco-Tipo: Hijo-Demonio.\n", getpid(), getppid());
        procesamiento();
    }

    else
    {
        if((hijo2 = fork()) == 0)
        {
            waitpid(hijo2,NULL,0);
            printf("PID: %d PPID: %d - Parentesco-Tipo: Hijo-Demonio.\n", getpid(), getppid());
            procesamiento();
        }
    }
}

/** -h cree 10 procesos hijos, donde cada uno realiza el mismo procesamiento que en el caso
anterior, el padre se quedará esperando a que se presione una tecla y en este momento se
cierran todos los procesos. */
void opcion_h()
{
    int padre = getpid();
    pid_t procesos[10];

    printf("PID: %d - Proceso Padre.\n", padre);

    for(int i=0; i<10; i++)
    {
        if((procesos[i] = fork())==0)
        {
            waitpid(procesos[i],NULL,0);
            printf("PID: %d PPID: %d - Parentesco-Tipo: Hijo-Normal.\n", getpid(), getppid());
            procesamiento();
        }
    }

    fflush(stdin);
    puts("Presione una tecla para continuar...");
    getchar();
/// Enviamos la señal para terminar los procesos HIJOS
    for(int i=0; i<10; i++)
    {
        kill(procesos[i],SIGKILL);
    }
}
/** -z cree 6 procesos nietos, de dos hijos distintos, y 2 de ellos deberán quedar como
zombies. El padre se quedará esperando a que se presione una tecla para poder verificar la
correcta generación de los procesos, y luego cierra los procesos. */
void opcion_z()
{
    int padre = getpid();
    pid_t hijo1,
          hijo2,
          nieto1_h1,
          nieto2_h1,
          nieto3_h1, ///Quedara como ZOMBIE
          nieto1_h2,
          nieto2_h2,
          nieto3_h2; ///Quedara como ZOMBIE

    printf("PID: %d - Proceso Padre.\n", padre);

    if((hijo1 = fork()) == 0)
    {
        waitpid(hijo1,NULL,0);
        printf("PID: %d PPID: %d - Parentesco-Tipo: Hijo-Normal.\n", getpid(), getppid());

        if((nieto1_h1 = fork())==0)
        {
            waitpid(nieto1_h1,NULL,0);
            printf("PID: %d PPID: %d - Parentesco-Tipo: Nieto-Normal.\n", getpid(), getppid());
        } /// Nieto 1 de Hijo 1
        else
        {
            if((nieto2_h1 = fork())==0)
            {
                waitpid(nieto2_h1,NULL,0);
                printf("PID: %d PPID: %d - Parentesco-Tipo: Nieto-Normal.\n", getpid(), getppid());
            } /// Nieto 2 de Hijo 1
            else
            {
                if((nieto3_h1 = fork())==0)
                {
                    printf("PID: %d PPID: %d - Parentesco-Tipo: Nieto-Zombie.\n", getpid(), getppid());
                    exit(0);
                } /// Nieto 3 de Hijo 1 - ZOMBIE
            }
        }
    } ///Hijo 1
    else
    {
        if((hijo2 = fork()) == 0)
        {
            waitpid(hijo2,NULL,0);
            printf("PID: %d PPID: %d - Parentesco-Tipo: Hijo-Normal.\n", getpid(), getppid());

            if((nieto1_h2 = fork())==0)
            {
                waitpid(nieto1_h2,NULL,0);
                printf("PID: %d PPID: %d - Parentesco-Tipo: Nieto-Normal.\n", getpid(), getppid());
            } /// Nieto 1 de Hijo 2
            else
            {
                if((nieto2_h2 = fork())==0)
                {
                    waitpid(nieto2_h2,NULL,0);
                    printf("PID: %d PPID: %d - Parentesco-Tipo: Nieto-Normal.\n", getpid(), getppid());
                } /// Nieto 2 de Hijo 2
                else
                {
                    if((nieto3_h2 = fork())==0)
                    {
                        printf("PID: %d PPID: %d - Parentesco-Tipo: Nieto-Zombie.\n", getpid(), getppid());
                        exit(0);
                    } /// Nieto 3 de Hijo 2 - ZOMBIE
                }
            }
        } ///Hijo 2
    }

    fflush(stdin);
    getchar();

    kill(nieto1_h1,SIGKILL);
    kill(nieto2_h1,SIGKILL);
    kill(nieto3_h1,SIGKILL);
    kill(nieto1_h2,SIGKILL);
    kill(nieto2_h2,SIGKILL);
    kill(nieto3_h2,SIGKILL);
    kill(hijo1,SIGKILL);
    kill(hijo2,SIGKILL);
}

void mostrar_ayuda()
{
    system("clear");
    puts("\t***\tAyuda para el Ejercicio 1\t***\n");
    puts("\n");
    puts("-d\t Se crearan dos procesos demonios, que deberán simplemente ejecutar cada 2 segundos");
    puts("\t un loop hasta 1.000.000 realizando una acumulación en una variable, y no finalizan");
    puts("\t simplemente para que generen algo de procesamiento.");
    puts("\n");
    puts("-h\t Se crearan 10 procesos hijos, donde cada uno realiza el mismo procesamiento que en el");
    puts("\t caso anterior [-d], el padre se quedará esperando a que se presione una tecla y en este");
    puts("\t momento se cierran todos los procesos.");
    puts("\n");
    puts("-z\t Se crearan 6 procesos nietos, de dos hijos distintos, y 2 de ellos deberán quedar como");
    puts("\t zombies. El padre se quedará esperando a que se presione una tecla para poder verificar");
    puts("\t la correcta generación de los procesos, y luego cierra los procesos.");
    puts("\n");
    puts("-a\t Muestra mensaje de ayuda");
    puts("\n");
    puts("USO: ./Ejercicio1 -d\n");
}
