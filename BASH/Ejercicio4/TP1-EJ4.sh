#!/bin/bash
# Script: TP1-EJ4.sh

# Trabajo Practico 1 - Ejercicio 4

# Integrantes del Equipo

# Amengual, Miguel 		35991055
# Baini, Hernan 		32883285
# Castagna, Cristian 		37398696
# Piacentini,Federico		36258738
# Scarpello, Franco Dario 	37842567

### F U N C I O N E S ###

function ayuda(){
	clear
	echo "SCRIPT: TP1-EJ4.sh"
	echo 'Controlar la similitud de un archivo base contra los archivos contenidos en un directorio'
	echo ""
	echo 'El script comparara el archivo pasado como parametro (OBLIGATORIO) contra cada uno de los archivos '
	echo 'encontrados en una rama de directorios tambien pasado parametro (OPCIONAL), informando los archivos '
	echo 'que tienen cierto porcentaje de igualdad (OBLIGATORIO). Si no se informa el directorio, tomara el '
	echo 'directorio actual.'
	echo ""
	echo 'Modo de llamada:'
	echo "$0"' [Nombre del archivo] [Directorio a comparar] [Porcentaje minimo de Igualdad]'
	echo ""
	echo 'OPCIONES:'
	echo '	-h,	-?,	--help	Muestra esta ayuda y finaliza'
}

### L O G I C A   P R I N C I P A L ###

# EVALUAR OPCIONES

OPTSPEC=":?h-"

while getopts "${OPTSPEC}" OPCION
do
	case "${OPCION}" in
		- )
			case "${OPTARG}" in
				help )
					ayuda
					exit 0
					;;
			esac
			;;
		
		h )
			ayuda
			exit 0
			;;
		\? )
			if [[ "${OPTARG}" != \? ]]
			then
				echo "La opcion ingresada es INCORRECTA. Puede revisar la ayuda usando -h, -? o --help."
				exit 1
			else
				ayuda
				exit 0
			fi
			;;
	esac
done

# Se valida la cantidad de parametros recibidos en la llamada

if [ "$#" -lt "1" ]
then
	echo "La cantidad de parametros es INCORRECTA. Puede revisar la ayuda usando -h, -? o --help"
	exit 1
fi

#Parametro 1: Archivo base a comparar [OBLIGATORIO]
#Parametro 2: Directorio de los archivos a comparar [OPCIONAL]
#Parametro 3: Porcentaje de igualdad [OBLIGATORIO]

declare ARCHIVOBASE="$1"

#Si viene solo 2 parametros asumimos que NO se envio el directorio a comparar
if [  "$#" -eq "2" ]
then
#Se asigna el directorio de ejecucion
	declare DIRECTORIO=`./`
	declare PORCENTAJE="$2"
#Sino asignamos a cada variable el valor recibido
else
	declare DIRECTORIO="$2""/*"
	declare PORCENTAJE="$3"
fi

#echo "Archivo Base: ""${ARCHIVOBASE}"
#echo "Directorio: ""${DIRECTORIO}"
#echo "Porcentaje de similitud: ""${PORCENTAJE}"

declare CANTIDADLINEAS

#Para cada archivo del DIRECTORIO enviado se realiza la comparacion con el ARCHIVOBASE
for ARCHIVOACTUAL in "$BUSCAR_ARCHIVO"$DIRECTORIO
do
##### BORRAR - prueba #########
	echo $ARCHIVOBASE
	echo $ARCHIVOACTUAL
#################################
#Obtengo la cantidad de lineas del archivo
         CANTIDADLINEAS=`wc -l ARCHIVOACTUAL`

#Obtengo la cantidad de lineas diferentes entre ARCHIVOBASE vs ARCHIVOACTUAL
#diff...

#Calculo el porcentaje de similitud como: [(Lineas Totales - Lineas Diferentes)*100]/Lineas Totales ---REVISAR DEFINICION

#Si el porcentaje obtenido es mayor o igual al parametro Porcentaje
#echo ARCHIVOACTUAL

done
