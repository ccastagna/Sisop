#! /bin/bash
# Script: Tp1-Ej1

# Trabajo Practico 1
# Ejercicio 1
# Entrega

# Integrantes del Equipo
# Franco Dario Scarpello 37842567
# Federico Piacentini 36258738
# Hernan Baini 32883285
# Miguel Amengual 35991055
# Cristian Castagna 37398696
#a)	¿Qué significa la línea “#!/bin/bash”?
#b)	¿Con qué comando y de qué manera otorgó los permisos de ejecución al script?
#c)	Explique el objetivo general del script.
#d)	Complete los echo "..." con mensajes acordes a cada situación según lo que se valida y
#e)	renombre la variable X con un nombre acorde a su utilidad.
#f)	Explique de manera general la utilidad de AWK.
#g)	¿Es posible ejecutar este script para procesar varios archivos a la vez en una única llamada? Ejemplifique.
#Respuestas:
#a)	Indica la ruta en donde se encuentra el interprete que se utilizará en nuestro script, en este caso, se encuentra en bin (directorio en donde se encuentra por defecto el intérprete de Bash que trae instalado el SO: Linux).
#b)	Se utilizó el comando: chmod 775 “nombreDelScript.sh”
#c)	El objetivo general es buscar dentro del archivo “condición tributaria con denominación Apellido y Nombre” (pasado a través del segundo parámetro) y mostrar por consola (en caso que exista el CUIT) el apellido, nombre y CUIT del CUIT pasado por el primer parámetro.
#d)	Hecho en el código.
#e)	Hecho en el código.
#f)	La utilidad de AWK es procesar texto con la sintaxis parecida al del lenguaje C, permitiendo operar sobre cada fila leída.
#g)	Si, se agrega al final de la línea de llamada del AWK otro $2.



if [ $# != 2 ]; then
echo "Error: No se ingresaron todos los parametros"
exit
fi
if ! [ -f "$2" ]; then
echo "Error: No es un archivo valido"
exit
fi
cuitToSearch=$1
awk 'BEGIN {
FIELDWIDTHS = "11 30"
}
$1 == "'$cuitToSearch'" {
print "Nombre                         CUIT"
print "------------------------------ ------------"
print $2" "$1
exit
}' $2

