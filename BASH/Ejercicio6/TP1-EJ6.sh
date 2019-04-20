#! /bin/bash
# TP1-EJ6
# Integrantes del Equipo
# Franco Dario Scarpello 37842567
# Federico Piacentini 36258738 
# Hernan Baini 32883285
# Miguel Amengual 35991055
# Cristian Castagna 37398696 

# Parameters rename
declare directory="$1"
declare pattern="$2"
declare text_replacement="$3"


# Declare Functions
function help(){
	echo "Script utilizado para renombrar todos los archivos de un directorio."
	echo ""
	echo "Uso: $0 [directorio] [patron] [texto a reemplazar] Renombra los archivos del directorio."
                        echo "Argumentos: "
                        echo "  -h o -? o --help      Impresión de la ayuda (este mensaje)."
}

function replacement(){
	declare replace="s/${pattern}/${text_replacement}/"
	declare new_filename="$(echo ${filename} | sed ${replace})"
	mv "${file_directory}/${filename}" "$(echo ${file_directory}/${new_filename})"
	filename="${new_filename}"
}


# Getopt Analize

# Option strings
#SHORT=h?
#LONG=help

# Read the options"
#OPTS=$(getopt -o :$SHORT --long $LONG -- "$@")

#eval set -- "${OPTS}"

# Handling the different allowed options
#while true
#do
#	case "$1" in
#		-h | --help )
#			help
#                       	exit 0
#                       	;;
#              	-? )
#			echo "La opcion ingresada no es valida. Ejecute $0 -h para mas informacion."
#			exit 1
#			shift;;
#       esac
#done

# Parameters Received Validation

if [ $# -ne "3" ]
then
	echo "El presente script espera recibir 3 parametros."
	echo "(Ejecuta $0 -h para mas ayuda)"
	echo ""
	exit 1
fi

if [ ! -d "${directory}"  ]
then
	echo "El directorio ingresado no existe."
	exit 1
fi

if [ ! "$(ls -A "${directory}")" ]
then
	echo "El directorio ingresado no contiene archivos."
	exit 1	
fi


# Filename replacement
declare counter=0

# Add IFS so the loop parse per each line (does not break because of a space in name)
IFS=$'\n'
for filename in $(find ${directory} -type f -name "*${pattern}*" -exec basename {} \;)
do
	file_directory=$(find ${directory} -name ${filename} -printf '%h\n')
	while [[ ${filename} == *"${pattern}"* ]]
	do
		replacement ${file_directory} ${filename} ${pattern} ${text_replacement}	
	done
	((counter++))	
done

echo "Se renombraron ${counter} archivo(s) de forma exitosa."

exit 0
