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

if ($existe -eq $true) {
    $lista = Get-ChildItem -File
    foreach ($item in $lista) {
        Write-Host “$($item.Name) $($item.Length)”
    }
    } else {
    Write-Error "El path no existe"
    }

<# 
a) ¿Cuál es el objetivo del script?

b) ¿Qué validaciones agregaría a la definición de parámetros?

c) ¿Con qué cmdlet se podría reemplazar el script para mostrar una salida similar?

#>