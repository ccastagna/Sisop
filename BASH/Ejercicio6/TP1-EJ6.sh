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
	mv "${file}" "$(echo ${file} | sed ${replace})"  
}

# GetOpts Analize

while getopts ":?h-" OPTION
do
        case $OPTION in
		- )
			case "${OPTARG}" in
				help )
					help
					exit 0
					;;
		esac
		;;

		h )
			help
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


# Parameters Received Validation
if [ $# -ne "3" ]
then
	echo "El presente script espera recibir 3 parametros."
	echo "(Ejecuta $0 -h para mas ayuda)"
	echo ""
	exit 1
fi

if [ ! -d "$1"  ]
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

for file in "${directory}/"*
do
	if [[ ${file} == *"${patron}"* ]]
	then
		((counter++))
	fi
	while [[ ${file} == *"${patron}"* ]]
	do
		replacement ${file} ${pattern} ${text_replacement} 
        done
	
done

echo "Se renombraron ${counter} archivo(s) de forma exitosa."

exit 0
