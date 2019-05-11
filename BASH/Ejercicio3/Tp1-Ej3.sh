#!/bin/bash
# Script: Tp1-Ej3

# Trabajo Practico 1
# Ejercicio 3
# Entrega

# Integrantes del Equipo
# Franco Dario Scarpello 37842567
# Federico Piacentini 36258738
# Hernan Baini 32883285
# Miguel Amengual 35991055
# Cristian Castagna 37398696
 

# Parameters description
# $1 Archivo de cartones

declare -r CANT_BOLILLAS=100
es_numero='^[0-9]+$'
declare -a bolillas
count_bolilla=0
cantaron_linea=0
ganadores_linea="Los cartones que cantaron linea son: "
ganadores_bingo="Los cartones que cantaron bingo son: "

# DECLARE FUNCTIONS

# FUNCTION help()
function help(){
echo "Script utilizado para jugar al bingo."
echo "Uso:"
echo "Se recibe un unico parametro que indica el archivo de donde se encuentran almacenados los cartones para jugar."
echo "En cada linea del archivo debe estar el numero de carton y 15 numeros separados por Tabs, cada 5 numeros representan una linea del carton."
echo "Los numeros que pueden salir son del 0 al 99."
echo "Luego de validar los cartones con los que se jugara, el script queda en espera de recibir la signal SIGUSR1."
echo "Cada vez que se recibe la signal, se saca una bolilla y se verifican los cartones a ver si hay algun ganador."
echo "Al finalizar, se muestra por pantalla un resumen de cantidad de bolillas sorteadas y los numero de carton ganadores de linea y bingo."
echo "Ejecucion:"
echo "1) Ejecutar el script en una terminal"
echo "2) En otra terminal ejecutar: kill -SIGUSR1 PID , donde PID es el ID del proceso de la primer terminal"
}

# FUNCTION validate_carton()
# Valida un carton creado a partir del archivo, para verificar que tenga un formato correcto
function validate_carton(){
declare carton=$1
declare count=0
for x in $carton;
do
        if ! [[ $x =~ $es_numero ]]
        then
                break
        fi
        if [[ $x -lt 0 ]] || [[ $x -gt $(($CANT_BOLILLAS-1)) ]]
        then
                break
        fi
        just_once=0
        repeated=0
        first=1
        for y in $carton;
        do
		if ! [[ $y =~ $es_numero ]]
        	then
               		break
        	fi
                if [[ first -eq 0 ]]
                then
                        if [[ $x -eq $y ]]
                        then
                                if [[ $just_once -eq 0 ]]
                                then
                                        just_once=1
                                else
                                repeated=1
                                fi
                        fi
                else
                        first=0
                fi
        done
        if [[ repeated -eq 1 ]]
        then
                break
        fi
        ((count++))
done
if [[ count -ne 16 ]]
then
        echo "El archivo no tiene un formato valido."
        echo "Error en el carton: " $carton
        exit 1
fi
}

# FUNCTION obtener_bolilla()
# Saca una bolilla aleatoria entre 0 y 99
# La ultima bolilla sacada queda en $bolilla y se acumulan todas en $bolillas
# La cantidad de bolillas sacadas hasta ahora esta en $count_bolilla
function obtener_bolilla(){

if [[ $count_bolilla -gt $(($CANT_BOLILLAS-1)) ]]
then
        echo "ERROR. Ya no hay mas bolillas para sacar"
        exit 1
fi

random=$(((RANDOM%$CANT_BOLILLAS)))
count=0

while [ $count -lt $count_bolilla ];
do
        if [[ ${bolillas[$count]} == $random ]]
        then
                obtener_bolilla
                return 1
        fi
        ((count++))
done

bolillas[$count_bolilla]=$random
((count_bolilla++))
bolilla=$random
echo "Salio la bolilla " $bolilla
}

# FUNCTION actualizar_cartones
# Se marca la ultima bolilla que salio en los cartones cargados con una X a la izquierda del valor
# Se verifica si algun carton saca linea o bingo
function actualizar_cartones(){
hubo_ganador=0
for (( position=0; position<=$count_cartones-1; position++ ))
do
        position_in_carton=0
        anotaron=0
        carton_editado=""
        nro_carton=""
        for x in ${cartones[$position]};
        do
                if [[ position_in_carton -ne 0 ]]
                then
                        if [[ anotaron -eq 0 ]] && [[ $x =~ $es_numero ]] &&  [[ $x -eq $bolilla ]]
                        then
                                x="X$x"
                                anotaron=1
                        fi
                        carton_editado+=$'\t'$x
                else
                        nro_carton+=$x
                        carton_editado+=$x
                fi
                ((position_in_carton++))
        done
        if [[ anotaron -eq 1 ]]
        then
                cartones[$position]="$carton_editado"
                echo "Anoto el carton " $nro_carton
                verificar_ganador
        fi
done
if [[ hubo_ganador -eq 1 ]]
then
        if [[ cantaron_linea -eq 0 ]]
        then
                cantaron_linea=1
        else
                echo "Se sortearon " $count_bolilla " bolillas."
                echo $ganadores_linea
                echo $ganadores_bingo
                echo "Se acabo el juego."
                exit 1
        fi
fi
}


# FUNCTION verificar_ganador
# Se llama luego de que un jugador anota. Verifica si el $carton_editado canta linea o bingo
function verificar_ganador(){
OIFS=$IFS
IFS=$'\t'
carton_array=($carton_editado)
if [[ cantaron_linea -eq 0 ]]
then
        if [[ ! ${carton_array[1]} =~ $es_numero ]] && [[ ! ${carton_array[2]} =~ $es_numero ]] &&      [[ ! ${carton_array[3]} =~ $es_numero ]] && [[ ! ${carton_array[4]} =~ $es_numero ]] && [[ ! ${carton_array[5]} =~ $es_numero ]]
        then
                echo "El carton " ${carton_array[0]} " canto linea."
                ganadores_linea+="${carton_array[0]} "
                hubo_ganador=1
        fi
        if [[ ! ${carton_array[6]} =~ $es_numero ]] && [[ ! ${carton_array[7]} =~ $es_numero ]] &&      [[ ! ${carton_array[8]} =~ $es_numero ]] && [[ ! ${carton_array[9]} =~ $es_numero ]] && [[ ! ${carton_array[10]} =~ $es_numero ]]
        then
                echo "El carton " ${carton_array[0]} " canto linea."
                ganadores_linea+="${carton_array[0]} "
                hubo_ganador=1
        fi
        if [[ ! ${carton_array[11]} =~ $es_numero ]] && [[ ! ${carton_array[12]} =~ $es_numero ]] &&    [[ ! ${carton_array[13]} =~ $es_numero ]] && [[ ! ${carton_array[14]} =~ $es_numero ]] && [[ ! ${carton_array[15]} =~ $es_numero ]]
        then
                echo "El carton " ${carton_array[0]} " canto linea."
                ganadores_linea+="${carton_array[0]} "
                hubo_ganador=1
        fi
else
        if [[ ! ${carton_array[1]} =~ $es_numero ]] && [[ ! ${carton_array[2]} =~ $es_numero ]] && [[ ! ${carton_array[3]} =~ $es_numero ]] && [[ ! ${carton_array[4]} =~ $es_numero ]] && [[ ! ${carton_array[5]} =~ $es_numero ]] && [[ ! ${carton_array[6]} =~ $es_numero ]] && [[ ! ${carton_array[7]} =~ $es_numero ]] &&      [[ ! ${carton_array[8]} =~ $es_numero ]] && [[ ! ${carton_array[9]} =~ $es_numero ]] && [[ ! ${carton_array[10]} =~ $es_numero ]] && [[ ! ${carton_array[11]} =~ $es_numero ]] && [[ ! ${carton_array[12]} =~ $es_numero ]] &&  [[ ! ${carton_array[13]} =~ $es_numero ]] && [[ ! ${carton_array[14]} =~ $es_numero ]] && [[ ! ${carton_array[15]} =~ $es_numero ]]
        then
                echo "El carton " ${carton_array[0]} " canto bingo."
                ganadores_bingo+="${carton_array[0]} "
                hubo_ganador=1
        fi
fi
IFS=$OIFS
}

function jugar_bingo(){
obtener_bolilla
actualizar_cartones
}

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
                                echo "Opcion ingresada invalida, ejecute $0 -h para mas infomracion."
                                exit 1
                        else
                                help
                                exit 0
                        fi
                        ;;
esac
done

# TRAP CTRL C
trap '' 2
trap jugar_bingo SIGUSR1

# VALIDATE THE PARAMETERS

if [[ $# -ne 1 ]]
then
        echo "El presente script espera recibir un unico parametro."
        exit 1
fi

if [[ ! -f "$1" ]]
then
        echo "No se encontro el archivo enviado por parametro."
        exit 1
fi

# READ THE FILE
declare -a cartones
exec 10<&0
exec < "$1"
let count_cartones=0
while read carton; do
        validate_carton "$carton"
        cartones[$count_cartones]=$carton
        ((count_cartones++))
done

echo Empezo el bingo
echo Numero de participantes: ${#cartones[@]}

for (( position=0; position<=$count_cartones-1; position++ ))
do
        echo "Carton $(($position+1)):"
        echo ${cartones[$position]} | awk -F' ' '{print $1}'
done


while true
do
        sleep 2
done

exit 0
