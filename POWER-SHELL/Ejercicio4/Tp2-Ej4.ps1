# Script Tp2-Ej4

# Trabajo práctico 2
# Ejercicio 4
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
	El script comprime en todos los logs de un directorio que contengan cierta cadena.
	.DESCRIPTION
	El script busca en dentro de todos los logs sin importar su extension del directorio pathLogs la cadena pasada como parametro.
	La salida sera un archivo comprimido con todos los logs que contenian dicha cadena.
	.PARAMETER pathLogs
    El parametro pathLogs espera recibir el directorio en el cual se buscaran recursivamente los logs.
    .PARAMETER pathSalida
    El parametro pathSalida espera recibir el fullPath del archivo .zip en el cual se comprimiran los logs.
    .PARAMETER cadena
	El parametro cadena espera recibir el patron que se buscara dentro de los logs.
	.EXAMPLE
	./TP1-EJ4.ps1 -pathLogs <path> -pathSalida <path\file.zip> -cadena <patron>/
#>

param(
	[parameter(Mandatory=$true)]
	[ValidateScript({Test-Path $_ -PathType 'Container'})]	#el directorio no existe
    [String] $pathLogs,
    [parameter(Mandatory=$true)]
	[ValidateScript({-Not ( Test-Path $_ )})]	#el archivo ya existe
	[ValidateScript( {$_ -match "\.zip" })]		#el archivo debe ser .zip
    [String] $pathSalida,
    [parameter(Mandatory=$true)]
    [ValidateNotNullOrEmpty()]          #la cadena no puede ser nula
    [String] $cadena
    )
    
<#
Se buscan todos los archivos recursivamente | 
se filtran todos los que contengan la cadena | 
se agrupan los directorios | 
se seleccionan los nombres de los directorios | 
se agrega cada directorio al archivo comprimido    
#>
Get-ChildItem $pathLogs -Recurse | Select-String -Pattern $cadena | Group-Object Path | Select-Object Name | ForEach-Object {
    $filepath = $_.Name;
    
    if( Test-Path $pathSalida){
        Compress-Archive $filepath -Update $pathSalida  
    }else{
        Compress-Archive $filepath $pathSalida
    }
}