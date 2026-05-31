# Informe de Testing - Gym Tracker en C

## Resultado general

Se ejecutaron 21 tests automatizados sobre la aplicación.

Resultado:

```text
Ran 21 tests

OK

Esto indica que la aplicación compila correctamente, se ejecuta sin errores en los casos testeados y las funciones principales responden como se espera.

Áreas testeadas
1. Menú principal

Se verificó que el menú permita salir correctamente y rechace opciones inválidas.

2. Cálculo de volumen

Se testeó la fórmula:

volumen = series * repeticiones * peso

También se validaron entradas incorrectas como series en cero, repeticiones en cero y peso negativo.

3. Múltiples ejercicios

Se verificó que el programa permita ingresar varios ejercicios, calcular el volumen individual y sumar el volumen total.

4. Ejercicios con ID centinela

Se testeó que el programa finalice correctamente cuando el usuario ingresa ID igual a 0.

5. Promedio de pesos

Se verificó la carga de pesos, el ordenamiento y el cálculo del promedio.

6. Peso máximo y mínimo

Se testeó la detección del peso máximo, mínimo y sus posiciones.

7. Búsqueda de peso

Se verificó la búsqueda de un peso dentro de una lista ordenada.

8. Registro de nombres y pesos

Se testeó el registro de ejercicios con nombre y peso utilizado.

9. Escritura en CSV

Se verificó que el programa cree el archivo CSV, escriba encabezado y guarde ejercicios con ID, fecha, sets, reps, peso y volumen.

10. Lectura de CSV

Se verificó que el programa lea el archivo CSV e imprima el contenido en consola.

11. Funciones auxiliares

Se probaron funciones internas como:

swap
quick_sort
binary_search
obtener_ultimo_id
volumen
Conclusión

Los tests validan los comportamientos principales del programa: cálculo de volumen, manejo de múltiples ejercicios, validación de entradas, ordenamiento, búsqueda, escritura y lectura de archivos CSV.


---

### 7. Archivo de ejemplo CSV

No subas tu `gym.csv` real si es generado. Mejor creá:

```text
examples/gym_sample.csv

Contenido:

id,fecha,ejercicio,sets,reps,peso,volumen
1,30/05/2026,Bench Press,3,10,60.00,1800.00
2,30/05/2026,Squat,4,8,100.00,3200.00
TOTAL,,,,,,5000.00