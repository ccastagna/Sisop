# TP2-EJ2

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
foreach ($file in Get-ChildItem -Path $path -Recurse -File) {
	$length = ($file | Measure-Object Length -Sum).Sum
	$name = $file.Name +"|"+ $length
	$exist = $false

	foreach($key in $hashTable.GetEnumerator()) {
		if ( $key.Name -eq $name){
			$hashTable[$key.Name] = $($key.Value + 1)
			$exist = $true
			break
		}
	}

	if ($exist -eq $false) {
		$hashTable.Add($name, 1)
	}
}
""
"Archivos repetidos dentro del directorio `"$path`"`:"

foreach($key in $hashTable.GetEnumerator()) {
	if($key.Value -gt 1){
		$($key.Name).Substring(0, $($key.Name).IndexOf('|'))
	}
}
""

exit 0