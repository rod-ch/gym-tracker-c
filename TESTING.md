# Informe de Testing - Gym Tracker en C

## Resultado general

Se ejecutaron **21 tests automatizados** sobre la aplicación.

Resultado:

```text
Ran 21 tests

OK
```

Esto indica que la aplicación compila correctamente, se ejecuta sin errores en los casos testeados y las funciones principales responden como se espera.

## Áreas testeadas

### 1. Menú principal

Se verificó que el menú permita salir correctamente y rechace opciones inválidas.

### 2. Cálculo de volumen

Se testeó la fórmula principal del programa:

```text
volumen = series * repeticiones * peso
```

También se validaron entradas incorrectas, como:

- series en cero;
- repeticiones en cero;
- peso negativo.

### 3. Cálculo de volumen de múltiples ejercicios

Se verificó que el programa permita ingresar varios ejercicios, calcular el volumen individual de cada uno y sumar el volumen total acumulado.

### 4. Ejercicios con ID centinela

Se verificó que el programa finalice correctamente cuando el usuario ingresa un ID igual a `0`.

### 5. Promedio de pesos

Se verificó que el programa pueda:

- cargar una lista de pesos;
- ordenar los valores;
- calcular el promedio.

### 6. Peso máximo y mínimo

Se testeó que el programa identifique correctamente:

- el peso máximo;
- el peso mínimo;
- la posición donde aparece cada valor.

### 7. Búsqueda de peso

Se verificó que el programa pueda buscar un peso específico dentro de una lista ordenada.

### 8. Registro de nombres y pesos

Se testeó el registro de ejercicios con nombre y peso utilizado.

### 9. Escritura en archivo CSV

Se verificó que el programa pueda:

- crear el archivo CSV si no existe;
- escribir el encabezado;
- guardar ejercicios con ID, fecha, nombre, sets, reps, peso y volumen;
- calcular y guardar el volumen total.

### 10. Lectura de archivo CSV

Se verificó que el programa pueda leer el archivo CSV e imprimir su contenido en consola.

### 11. Funciones auxiliares internas

También se probaron funciones internas mediante un programa auxiliar en C.

Funciones testeadas:

- `swap`
- `quick_sort`
- `binary_search`
- `obtener_ultimo_id`
- `volumen`

## Conclusión

Los tests validan los comportamientos principales del programa:

- menú interactivo;
- cálculo de volumen;
- manejo de múltiples ejercicios;
- validación de entradas;
- ordenamiento de pesos;
- búsqueda binaria;
- cálculo de promedio;
- detección de máximo y mínimo;
- registro de ejercicios;
- escritura y lectura de archivos CSV;
- funciones auxiliares internas.

El resultado `OK` indica que todos los tests pasaron exitosamente.