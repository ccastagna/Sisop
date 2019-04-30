#! /bin/bash
# Script: TP1-EJ2

# Trabajo Practico 1
# Ejercicio 2
# Entrega

# Integrantes del Equipo
# Franco Dario Scarpello 37842567
# Federico Piacentini 36258738
# Hernan Baini 32883285
# Miguel Amengual 35991055
# Cristian Castagna 37398696


# Parameters Description
# $1: modificador:
#      -c para cuit
#      -d para dni
#      -n para nombre o parte del mismo
# $2: Nro de cuit o nro de dni o nombre o parte del mismo
# $3: direccion del archivo de la AFIP.

# Global Variable
typeOfValueToSearch=$1
valueToSearch=$2
fileAFIP=$3

# Declare Functions
function help(){
        echo "Script utilizado para filtrar por dni cuit o nombre el archivo de contribucion tributaria con denominacion "
        echo ""
        echo "Uso: $0 [modificador] [cuit|dni|nombre] [archivo AFIP]."
	echo "Modificadores:"
	echo "	-d para dni"
	echo "	-c para cuit"
	echo "	-n para nombre"
                        echo "Argumentos: "
                        echo "  -h o -? o --help      Impresión de la ayuda (este mensaje)."
	echo "Es nacesario dar permisos de escritura y ejecucicion, ejecute el comando: chmod 775 'nombreDelScript' "
}

function validateParameterMode(){
	if [[ $typeOfValueToSearch != "-d" && $typeOfValueToSearch != "-c" && $typeOfValueToSearch != "-n"  ]]; then
		echo "Parametro incorrecto: utilice -d para dni, -c para cuit o -n para nombre"
		exit 1
	fi
}

function validateLength(){
	if [[ $typeOfValueToSearch = "-c" && ${#valueToSearch} -ne "11" ]]; then
		echo "Nro de cuit invalido"
		exit 2
	fi
	if [[ $typeOfValueToSearch = "-d" && ${#valueToSearch} -ne "8" ]]; then
                echo "Nro de dni invalido"
                exit 2
        fi	
}

function searchInFile(){
	awk -v valueToSearch="$valueToSearch" 'BEGIN {
		FIELDWIDTHS = "11 30 2 2 2 1 1 2";
		counter = 0
		print "------------------------------ ----------- ------------- ------- ----------- -------------- ------------------- -----------"
                print "Nombre                         CUIT        Imp ganancias Imp IVA Monotributo Integrante soc Empleadro Actividad Monotributo"
                print "------------------------------ ----------- ------------- ------- ----------- -------------- ------------------- -----------"			
	}
	{	
		if ( "'$typeOfValueToSearch'" == "-d" && substr($1,3,8) == valueToSearch) {
                       	print $2" "$1" "$3"            "$4"      "$5"          "$6"              "$7"                   "$8
			counter++
		} else if ( "'$typeOfValueToSearch'" == "-c" && $1 == valueToSearch) {
			print $2" "$1" "$3"            "$4"     "$5"      "$6"               "$7"                       "$8
			counter++
		} else 	if ( "'$typeOfValueToSearch'" == "-n" && $2 ~ valueToSearch ) {
			print $2" "$1" "$3"            "$4"      "$5"          "$6"              "$7"                   "$8
			counter++
		}
	}
	END{
                print "---------------------------------------------------------------------------------------------------------------------------"
		print "Cantidad De Registros Filtados: "counter
	}' "$fileAFIP"
}

function validateStructure(){
	if [ $1 != 3 ]; then
		echo "Error: Ingrese todos los parametros"
		help
		exit
	fi
	if ! [ -f "$fileAFIP" ]; then
		echo "Error: No es un archivo de texto válido"
		exit
	fi
}

# GetOpts Analize
while getopts ":c:d:n:?h-:" OPTION
do

	case $OPTION in
		- )
		case "${OPTARG}" in
				help )
					help
					exit 0
					;;
				* )
					echo "Opcion ingresada invalida, ejecute $0 -h para mas informacion."
					exit 1
					;;
                esac
                ;;

                h )
                        help
                        exit 0
                        ;;
		c | d | n )    	 
			validateStructure $#
			validateLength
			searchInFile
			exit 0
			;;

		\? )
                    	if [[ "${OPTARG}" != \? ]]
                        then
				echo "Opcion ingresada invalida, ejecute $0 -h para mas informacion."
                                exit 1
                        else
                                help
                                exit 0
                        fi
		;;
	esac
done
