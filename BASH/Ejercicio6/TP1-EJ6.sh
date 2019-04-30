#! /bin/bash
# TP1-EJ6
# Integrantes del Equipo
# Franco Dario Scarpello 37842567
# Federico Piacentini 36258738 
# Hernan Baini 32883285
# Miguel Amengual 35991055
# Cristian Castagna 37398696 

# Parameters rename
directory="$1"
pattern="$2"
text_replacement="$3"

#Declare Functions
function help(){
	echo "Script utilizado para renombrar todos los archivos de un directorio dado "
	echo "un directorio ingresado, el patron a reemplazar y el text con el cual"
	echo "reemplazar al patron."
	echo ""
	echo "Uso: $0 [directorio] [patron] [texto a reemplazar] Renombra los archivos del directorio."
                        echo "Argumentos: "
                        echo "  -h o -? o --help      Impresión de la ayuda (este mensaje)."
			echo ""
}

function replacement(){
	replace="s/${pattern}/${text_replacement}/"
	new_filename="$(echo ${filename} | sed ${replace})"
	mv "${file_directory}/${filename}" "$(echo ${file_directory}/${new_filename})"
	filename="${new_filename}"
}

# Getopts Analize
OPTSPEC=":?h-:"

while getopts "${OPTSPEC}" OPTION
do
	case "${OPTION}" in
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

if [ ! -d "${directory}"  ]
then
	echo "El directorio ingresado no existe."
	echo ""
	exit 1
fi

if [ ! "$(ls -A "${directory}")" ]
then
	echo "El directorio ingresado no contiene archivos."
	echo ""
	exit 1	
fi


# Filename replacement
counter=0

# Add IFS so the loop parse per each line (does not break because of a space in name)
IFS=$'\n'

for file_and_directory in $(find ${directory} -type f -name "*${pattern}*")
do
	filename=$(basename "${file_and_directory}")
	file_directory=$(dirname "${file_and_directory}")
	change_flag=false
	while [[ ${filename} == *"${pattern}"* ]]
	do
		replacement
		change_flag=true
	done
	if [[ change_flag -eq "true"  ]]
	then
		((counter++))	
	fi
done

echo "Se renombraron ${counter} archivo(s) de forma exitosa."
echo ""

exit 0
