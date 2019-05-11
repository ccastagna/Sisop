# Script Tp2-Ej1

# Trabajo práctico 2
# Ejercicio 1
# Entrega

# Integrantes del Equipo
# Franco Dario Scarpello 37842567
# Federico Piacentini 36258738 
# Hernan Baini 32883285
# Miguel Amengual 35991055
# Cristian Castagna 37398696

Param($pathsalida)
$existe = Test-Path $pathsalida

Write-Host "${pathsalida}"

if ($existe -eq $true) {
    $lista = Get-ChildItem -File
    foreach ($item in $lista) {
        $item.TotalItemSize
        Write-Host "$($item.Name) $($item.Length)"
    }
    } else {
    Write-Error "El path no existe"
    }

<# 
a) ¿Cuál es el objetivo del script?
El objetivo de este script es: Si el directorio brindado existe, listar nombre de los archivos del
path donde se encuentra el script seguido de su tamaño (no es recursivo). 
En caso de que no exista el directorio que llega por parámetro, indica "El path no existe".

Entendemos que hay un defecto en el script brindado, dado que en la línea:
$lista = Get-ChildItem -File
no se hace referencia al $pathsalida recibido, y por este motivo siempre se listarían los archivos del directorio
donde se encuentra el script corriendo.

Un posible fix sería:
$lista = Get-ChildItem -Path "${pathsalida}" -File

Si quisieramos que sea recursivo..
$lista = Get-ChildItem -Path "${pathsalida}" -File -Recurse

b) ¿Qué validaciones agregaría a la definición de parámetros?
Validaría directamente sobre el parámetro si existe el directorio, si contiene archivos y si es un string:

Param(
        [ValidateScript({Test-Path $_ -PathType 'Container'})]	#path does not exist
        [ValidateScript({Test-Path $_\* -PathType 'Leaf'})]		#path does not contain files
        [String] $pathsalida )

c) ¿Con qué cmdlet se podría reemplazar el script para mostrar una salida similar?
Select-Object brindaría una salida similar
Get-ChildItem -Path "F:\eclipse-workspace\Sisop\POWER-SHELL\Ejercicio1" -File | Select-Object Name, @{Name="Length";Expression={(Get-Item $_.name).length}}
#>