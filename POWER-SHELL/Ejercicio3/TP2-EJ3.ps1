# Script Tp2-Ej3

# Trabajo práctico 2
# Ejercicio 3
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
    El script recibe un archivo .csv de entrada con el formato "Patente","ValorMulta","Fecha"; debe 
    totalizar los valores de todas la multas acumulando por PATENTE-AÑO y generar un archivo de 
    salida .csv con el formato "Patente","Año","TotalMultas".
.DESCRIPTION
    El script recibe 2 parametros:
        -Nombre del Archivo de Entrada
        -Nombre del Archivo de Salida

    El resultado obtenido se guardará en un nuevo archivo con el nombre indicado como Salida
.PARAMETER Entrada
    El parametro Entrada espera recibir el archivo de entrada.
.PARAMETER Salida
    El parametro Salida espera recibir el nombre del archivo de salida.
.EXAMPLE
    ./TP2-EJ3.ps1 -Entrada ./entrada.csv -Salida salida.csv
#>

# Validacion de parametros
param(
# Entrada
	[parameter(Mandatory=$true)]	#Archivo de entrada es OBLIGATORIO
    [String] $Entrada,
# Salida
    [parameter(Mandatory=$true)]    #Archivo de salida es OBLIGATORIO
    [String] $Salida
) 

#Logica principal

#Chequear si el archivo de salida recibido YA EXISTE
if (Test-Path $salida ) {
    Write-Host "El archivo de salida recibido:" $salida "YA EXISTE. Verificar!" -ForegroundColor Red
    exit 1
}

#Chequear si el archivo de entrada recibido EXISTE
try {
# Importar contenido
    $importado = Import-CSV -Path $Entrada -Delimiter ','
}
catch [System.IO.FileNotFoundException]{
    Write-Host "El archivo:" $Entrada "NO EXISTE. Verificar llamada!" -ForegroundColor Red
    exit 1
}

# Ordenar registros por PATENTE y FECHA
$ordenado = $importado | Sort-Object -Property Patente, @{expression={$_.Fecha -as [datetime]}}

# Inicializar variables
$patente  = 0
$añoAnt   = 0
$saldoAño = 0
$resumen  = @()

# Procesar registros
$ordenado | ForEach-Object {
    $fechaActual = [datetime]::ParseExact($_.Fecha, "dd/mm/yyyy", $null)
    $añoActual = $fechaActual.year

    if ($patente -eq $_.Patente) {
#   Mientras sea la misma patente 
        # Revisamos si es el mismo año para acumular
        if ($añoAnt -eq $añoActual) {
            # Acumulo el valor de la multa
            $saldoAño += [int]$_.ValorMulta
        }
        else {
        # Cambia año
            # Guardar registro anterior
            $nuevoRegistro = New-Object System.Object
            $nuevoRegistro | Add-Member -MemberType NoteProperty -Name "Patente" -Value $Patente
            $nuevoRegistro | Add-Member -MemberType NoteProperty -Name "Año" -Value $añoAnt
            $nuevoRegistro | Add-Member -MemberType NoteProperty -Name "TotalMultas" -Value $saldoAño
        
            $resumen += $nuevoRegistro

            # Actualizar variables
            $añoAnt = $añoActual            
            $saldoAño = [int]$_.ValorMulta
        }
    }
    else {
#   Cambia patente
        if ($patente -ne 0) {
            #Guardar registro anterior
            $nuevoRegistro = New-Object System.Object
            $nuevoRegistro | Add-Member -MemberType NoteProperty -Name "Patente" -Value $Patente
            $nuevoRegistro | Add-Member -MemberType NoteProperty -Name "Año" -Value $añoAnt
            $nuevoRegistro | Add-Member -MemberType NoteProperty -Name "TotalMultas" -Value $saldoAño
        
            $resumen += $nuevoRegistro
        }
        # Actualizar variables
        $añoAnt = $añoActual            
        $saldoAño = [int]$_.ValorMulta
        $patente = $_.Patente
    }
}

# Ultimo registro
if ($patente -ne 0) {
    $nuevoRegistro = New-Object System.Object
    $nuevoRegistro | Add-Member -MemberType NoteProperty -Name "Patente" -Value $Patente
    $nuevoRegistro | Add-Member -MemberType NoteProperty -Name "Año" -Value $añoAnt
    $nuevoRegistro | Add-Member -MemberType NoteProperty -Name "TotalMultas" -Value $saldoAño

    $resumen += $nuevoRegistro
}

if ($resumen -ne $null) {
    try {
        $resumen | Export-Csv -NoTypeInformation -Path $Salida
        Write-Host "Los datos fueron procesados correctamente. Se genero el archivo resumen:" $Salida -ForegroundColor Green
        exit 0           
    }
    catch [System.IO.DirectoryNotFoundException]{
        Write-Host "No se pudo encontrar el directorio destino recibido:" $salida -ForegroundColor Red
        Write-Host "Los registros procesados no fueron guardados." -ForegroundColor Red
        exit 1
    }

}
else {
    Write-Host "NO se generaron registros a guardar. Verificar archivo de entrada" -ForegroundColor Red
    exit 1
}