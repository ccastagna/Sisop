#! /bin/bash
# TP1-EJ2
# Integrantes del Equipo
# Franco Dario Scarpello 37842567
# Federico Piacentini 36258738
# Hernan Baini 32883285
# Miguel Amengual 35991055
# Cristian Castagna 37398696

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

