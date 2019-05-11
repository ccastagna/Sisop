# Script Tp2-Ej6

# Trabajo práctico 2
# Ejercicio 6
# Entrega

# Integrantes del Equipo
# Franco Dario Scarpello 37842567
# Federico Piacentini 36258738 
# Hernan Baini 32883285
# Miguel Amengual 35991055
# Cristian Castagna 37398696

# Get-Help Script

<#
	.SYNOPSIS
	El script permite realizar producto escalar y transposición de matrices.
    .DESCRIPTION
    El script recibe 2 parametros:
        -Entrada
        -Producto o Transponer (No se pueden usar ambos a la vez)
    Si se recibe Producto, se debe recibir un numero entero, que va a ser el numero con el que se va a multiplicar la matriz de entrada
    Si se recibe Transponer, no se necesita recibir nada mas, realizará la transposicion de la matriz de entrada
    El resultado obtenido se guardará en un archivo que se llamará: “salida.nombreArchivoEntrada”
	.PARAMETER Entrada
    El parametro Entrada espera recibir el archivo de entrada.
    .PARAMETER Producto
    El parametro Producto espera recibir el escalar a ser utilizado en el producto escalar.
    .PARAMETER Transponer
	El parametro Transponer indica que se transpone la matriz de entrada.
	.EXAMPLE
    ./TP1-EJ6.ps1 -Entrada "path de entrada" -Producto 2
    .EXAMPLE 
    ./TP1-EJ6.ps1 -Entrada "path de entrada" -Transponer
#>
param(
	[parameter(Mandatory=$true)] 	    #filePath is a mandatory parameter
    [ValidateScript({Test-Path $_})]    #filePath is a valid File
    [string]                            #filePath is a string
    $filePath      
    )					

Write-Output "$filePath"