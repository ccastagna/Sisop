# Script Tp2-Ej5

# Trabajo practico 2
# Ejercicio 5
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
El script informa por pantalla cada 3 segundos la cantidad de procesos que se encuentran corriendo o la cantidad de archivos en un directorio.
.DESCRIPTION
El script realiza una de las siguientes acciones:  
 - Informar la cantidad de procesos que se encuentran corriendo en ese momento. 
 - Indicar la cantidad de archivos que contiene un directorio.  
El script puede recibir los siguientes parametros: 
 -Procesos: Parametro de tipo switch que indica que se mostrara la cantidad de procesos corriendo al momento de ejecutar el script. 
 -Archivos: Parametro de tipo switch que indica que se mostrara la cantidad de archivos que contiene un directorio. 
 -Directorio: Solo se puede usar si se paso Archivos. Indica el directorio a evaluar. 
Importante: 
Procesos y Archivos no se pueden usar al mismo tiempo. 
Directorio se puede usar solamente si Archivos se encuentra presente.
.PARAMETER Procesos
Parametro de tipo switch que indica que se mostrara la cantidad de procesos corriendo al momento de ejecutar el script. 
Importante: -Procesos y -Archivos no se pueden usar al mismo tiempo. 
.PARAMETER Archivos
Parametro de tipo switch que indica que se mostrara la cantidad de archivos que contiene un directorio. 
Importante: -Procesos y -Archivos no se pueden usar al mismo tiempo. 
.PARAMETER Directorio
Solo se puede usar si se paso -Archivos. Indica el directorio a evaluar. Si no se envia, se evaluara el directorio donde se encuentra el script.
.EXAMPLE
./TP1-EJ5.ps1 -Procesos
Informar la cantidad de procesos que se encuentran corriendo en ese momento. 
.EXAMPLE 
./TP1-EJ6.ps1 -Archivos -Directorio "path de directorio"
Indicar la cantidad de archivos que contiene un directorio.
#>


param(
    [parameter(Mandatory = $false, ParameterSetName = "Procesos")]
    [switch] $Procesos,               
    [parameter(Mandatory = $false, ParameterSetName = "Archivos")]
    [switch] $Archivos , 
    [parameter(Mandatory = $false, ParameterSetName = "Archivos")] 	    
    [ValidateScript( { Test-Path $_ })]    
    [string] $Directorio     
)		

if ($Procesos -eq $true) {
    while ($true) {
        Write-Host $(Get-Process).count
        Start-Sleep -s 3
    }
}
elseif ($Archivos -eq $true) { 
    while ($true) {
        Get-ChildItem -Path $Directorio -File | Measure-Object | %{$_.Count}
        Start-Sleep -s 3
    }
}
