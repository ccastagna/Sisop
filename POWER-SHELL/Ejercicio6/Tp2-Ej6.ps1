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
[parameter(Mandatory=$true)] 	    #Entrada is a mandatory parameter
[ValidateScript({Test-Path $_})]    #Entrada is a valid File
[string] $Entrada,                  #Entrada is a string
[parameter(Mandatory=$false, ParameterSetName="Producto")]
[int] $Producto,
[parameter(Mandatory=$false, ParameterSetName="Transponer")]
[switch] $Transponer     
)		

#ATRIBUTOS
$rows = 0;
$columns = 0;
$routeNewFile = "";

function Read-File-And-Create-Matrix(){
    Param([string] $Route)

    $Matrix = @(); #Crea una matriz vacia
    
    foreach($line in Get-Content $Route) { #por cada linea del texto de entrada
        #Agrego una nueva fila , reemplazando | por , para luego ejecutar un comando y poder agregar esa fila
        $Matrix += ,@( Invoke-Expression -Command $line.replace("|",",")); 
    }
    return $Matrix;
}

$Matrix = Read-File-And-Create-Matrix $Entrada

$rows = $Matrix.Length;

if ($rows -gt 0){
    $columns = $Matrix[0].Length;
}

function Create-Path-Name() {
    param (
        [String] $Entrada
    )
    $Path = Split-Path -Path $Entrada -Parent #ME DA EL DIRECTORIO DEL PARAMETRO SIN EL NOMBRE DEL ARCHIVO 
    $FileName = (Get-Item $Entrada | Select-Object basename)[0].BaseName #SOLO EL NOMBRE DEL ARCHIVO
    $global:routeNewFile = "$Path/salida.$FileName"
    return $routeNewFile;
}

function Create-Empty-File(){
    Param([String] $Entrada);
    Create-Path-Name $Entrada;
    if(Test-Path $global:routeNewFile){
        $global:routeNewFile = $null;
        return;
    }
   $null = New-Item -Path $global:routeNewFile -ItemType File; # $null es para ignorar todo lo que devuelva.
}

function Traspose-Matrix(){
    Param([Array] $Matrix , [int] $rows, [int] $columns);

    for($currentColumn = 0; $currentColumn -lt $columns; $currentColumn++){
        $newLine=""
        for($currentRow = 0; $currentRow -lt $rows; $currentRow++){
            $newLine += $Matrix[$currentRow][$currentColumn].ToString()+"|";
        }
        $newLine=$newLine.Substring(0,$newLine.Length-1)
        Add-Content $global:routeNewFile $newLine
    }  
}
function Scalar-product(){
    Param([Array] $Matrix , [int] $rows, [int] $columns, [int] $Producto);

    for($currentRow = 0; $currentRow -lt $rows; $currentRow++){
        $newLine=""
        for($currentColumn = 0; $currentColumn -lt $columns; $currentColumn++){
            $newLine += ($Matrix[$currentRow][$currentColumn]*$Producto).ToString()+"|";
        }
        $newLine=$newLine.Substring(0,$newLine.Length-1)
        Add-Content $global:routeNewFile $newLine
    }  
}

switch ($PsCmdlet.ParameterSetName) {
    "Producto" {
        Create-Empty-File $Entrada
        if($global:routeNewFile){
            Scalar-product $Matrix $rows $columns $Producto
            Write-Output "Se relizó el producto escalar en: $global:routeNewFile"
        }else{
            Write-Error "Ya existe el archivo destino"
        }
        break
    }
    "Transponer" {
        Create-Empty-File $Entrada
        if($global:routeNewFile){
            Traspose-Matrix $Matrix $rows $columns;
            Write-Output "Se creó la transpuesta en: $global:routeNewFile"
        }else{
            Write-Error "Ya existe el archivo destino"
        }
        break
    }
    Default {
        Write-Output "Opcion Invalida"
    }
}


