# Script Tp2-Ej2

# Trabajo práctico 2
# Ejercicio 2
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
	El script cuenta la cantidad de archivos duplicados que hay en un directorio.
	.DESCRIPTION
	El script considera archivos duplicados a aquellos que tienen el mismo nombre y peso, sin importar su contenido.
	La salida seran los nombres de los archivos duplicados(una sola vez por grupo de archivos duplicados) sin sus respectivos paths.
	.PARAMETER path
	El parametro path espera recibir el directorio en el cual realizar las comparaciones.
	.EXAMPLE
	./TP1-EJ2.ps1 archivos/
#>

# Parameters Validation
param(
	[parameter(Mandatory=$true)] 							#path is a mandatory parameter
	[ValidateScript({Test-Path $_ -PathType 'Container'})]	#path does not exist
	[ValidateScript({Test-Path $_\* -PathType 'Leaf'})]		#path does not contain files
	[String] $path)											#path is a string

# HashTable Instantiation
$hashTable = @{}

# Main Logic
$at_least_one_duplicated = $false
$files = Get-ChildItem -Path $path -Recurse -File


$files | ForEach-Object {
	$length = ($_ | Measure-Object Length)
	$name = $_.Name +"|"+ $length
	$exist = $false

	$hashTable.GetEnumerator() | ForEach-Object {
		if ( $_.Key -eq $name ){
			$duplicate_key = $_.Key
			$exist = $true
			$at_least_one_duplicated = $true
			return
		}
	}

	if( $exist -eq $false ){
		$hashTable.Add($name, 1)
	} else {
		$hashTable[$duplicate_key] += 1
	}
}

""
if ($at_least_one_duplicated -eq $true) {
	"Archivos repetidos dentro del directorio `"$path`"`:"

	$hashTable.GetEnumerator() | ForEach-Object {
		if($_.Value -gt 1){
			$($_.Name).Substring(0, $($_.Name).IndexOf('|'))
		}
	}
} else {
	"No se encontraron archivos repetidos dentro del directorio `"$path`" "
}


""

exit 0