#!/bin/bash
# Script: TP1-EJ5

# Trabajo Practico 1
# Ejercicio 5

# Integrantes del Equipo
# Franco Dario Scarpello 37842567
# Federico Piacentini 36258738
# Hernan Baini 32883285
# Miguel Amengual 35991055
# Cristian Castagna 37398696

# Entrega

DIR="$2"
DATE=`date "+%Y-%m-%d"`
TIME=`date "+%H-%M-%S"`
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)"/BackUps
EXTENSIONFILES=""

help(){
	echo 'Uso: TP-Ej5 [OPTION...] [DIRECTORY]'
	echo TP-Ej5 realiza un backup de los archivos del directorio especificado
	echo ""
	echo Ejemplos:
	echo '  TP-Ej5 -t /home/username/document/ #Hace un backup del directorio ./document'
	echo '  TP-Ej5 -x txt /home/username/document/ #Hace un backup todos los archivos con extension txt en ./document'
	echo ""
	echo OPCIONES:
	echo '	-h,	-?,	--help	Muestra esta lista de ayuda'
	echo '	-t			Hacer backup de todo un directorio'
	echo '	-x			espera una extension como argumento con el que filtrar los archivos para el backup'
}

validateDirectory(){
	if [ ! -d "$DIR" ]; then
                echo El directorio especificado no es valido
                exit 1
        fi
        if [[ "${DIR: -1}" != "/" ]]; then
                DIR="$DIR"/
        fi
}

validateExtension1(){
	FILESWITHEXTENSION=(`ls "$DIR"*."$1"`)
	echo $FILESWITHEXTENSION
	if [[ ! "$FILESWITHEXTENSION" || ${#FILESWITHEXTENSION[@]} -eq 0 ]]; then
                echo No hay archivos con esa extension en el directorio especificado
                exit 1
        fi	       
}


validateExtension(){
	EXT=$1
	
	if [[ -z `find "$DIR" -name *."$EXT" -print -quit` ]]; then
                echo No hay archivos con esa extension en el directorio especificado
                exit 1
	fi
}

validateBackupDirectories(){
	if [ ! -d "$SCRIPTDIR" ]; then
                mkdir "$SCRIPTDIR"
        fi

        if [ ! -d "$SCRIPTDIR/"$1"BackUps" ]; then
                mkdir "$SCRIPTDIR/"$1"BackUps"
        fi
}

generateLogContent(){
	echo Usuario: $USER
	echo Fecha: $DATE
	echo Hora: $TIME
	echo ""
	echo Archivos:
	for LINE in `tar -tf "$1".tar.gz` 
	do
	       echo "$LINE"
       	done	       
}


compressAll(){

	TARFILENAME="$DATE"_"$TIME"

	validateDirectory

	case "$DIR" in 
		/*)	
			tar -cf "$TARFILENAME".tar --absolute-names "$DIR"
			;;
		*)
			tar -cf "$TARFILENAME".tar "$DIR"
	esac

	gzip "$TARFILENAME".tar

	validateBackupDirectories "Full"    

	mv  "$TARFILENAME".tar.gz ""$SCRIPTDIR"/FullBackUps"
		
	cd ""$( dirname "${BASH_SOURCE[0]}")"/BackUps/FullBackUps"

	generateLogContent $TARFILENAME 1> $TARFILENAME.txt 
}

compressByExtension(){

        TARFILENAME="$DATE"_"$TIME""$1"

        validateDirectory
	validateExtension "$1"

	case "$DIR" in
                /*)
			find "$DIR" -name "*.""$1" -exec tar -rvf "$TARFILENAME".tar {} \; 2>/dev/null
                        ;;
                *)
			find "$DIR" -name "*.""$1" -exec tar -rvf "$TARFILENAME".tar {} \;

        esac

	gzip "$TARFILENAME".tar

        validateBackupDirectories $1

        mv  "$TARFILENAME".tar.gz ""$SCRIPTDIR"/"$1"BackUps"

        cd ""$( dirname "${BASH_SOURCE[0]}")"/BackUps/"$1"BackUps"

	generateLogContent $TARFILENAME 1> $TARFILENAME.txt
}


deleteOlderBackup(){
	BACKUPFILES=(`ls *.tar.gz -t`)

	if [ ${#BACKUPFILES[@]} -gt 5 ]; then
		local FILEBASENAME="$( echo "${BACKUPFILES[-1]}" | cut -f 1 -d '.')"
	
		echo eliminando "${BACKUPFILES[-1]}"
		echo eliminando "$( echo "${BACKUPFILES[-1]}" | cut -f 1 -d '.')".txt
	
		rm ""$FILEBASENAME".tar.gz"
		rm ""$FILEBASENAME".txt"
	fi	
}


if [[ "$#" -eq "0" || "$#" -gt "3" ]]; then
	echo "Cantidad de atributos incorrecta"
	exit 1
fi

OPTSPEC=":tx:?h-"
while getopts "$OPTSPEC" OPTCHAR; do
	case "${OPTCHAR}" in
		- )
			case "${OPTARG}" in
				help )
					help
					exit 0 
					;;
			esac
			;;
		t )
			compressAll
			deleteOlderBackup
			exit 0
			;;
		x )
			DIR="$3"
			compressByExtension "${OPTARG}"
			deleteOlderBackup
	      	        exit 0 
			;;
		h ) 	
			help
			exit 0
			;;
		\? )
			if [[ "${OPTARG}" != "\?" ]]; then

				echo "la opcion no es correcta"
				exit 1
			
			fi
			
			help
			exit 0
			;;
	esac
done

