VibeNodes

ELO-320 – Estructuras de Datos y Algoritmos
Tarea 2

1. Descripción General

VibeNodes es un sistema de gestión y análisis de datos musicales que integra múltiples estructuras de datos vistas en el curso, con el objetivo de evaluar su desempeño teórico y empírico bajo distintas cargas de datos.

El sistema permite:

Autenticación y registro de usuarios.

Las principales estructuras utilizadas son:

Tabla Hash para gestión de usuarios.
Árbol AVL para almacenamiento y búsqueda de artistas.


2. Instrucciones de Compilación y Ejecución
2.1 Compilación

El proyecto se compila utilizando make, el cual genera el ejecutable vibenodes.

make
2.2 Ejecución

Formato general del programa:

./vibenodes <modo_lista> <modo_usuario> <directorio_datos>
Parámetros
<modo_grafo>
-lista: grafo implementado mediante lista de adyacencia.
<modo_usuario>
-user: inicia en modo usuario estándar (login/registro).
<directorio_datos>
Ruta al directorio que contiene los archivos CSV:
usuarios.csv
catalogo.csv
spotify_tracks_raw.csv
reproducciones.csv

Ejemplos de invocación
./vibenodes -lista -user data/
./vibenodes -matriz -admin data/
3. Análisis de Complejidad
3.1 Tabla Hash (Usuarios) 
Inserción: O(1) promedio
Búsqueda: O(1) promedio
Borrar: O(1) promedio

3.2 Árbol AVL (Artistas)
Inserción: O(log N)
Búsqueda: O(log N)
Eliminación: O(log N)

El árbol AVL mantiene los artistas ordenados alfabéticamente y se rebalancea automáticamente tras cada inserción o eliminación.


5. Pruebas de Eficiencia

Se realizaron pruebas con al menos cuatro instancias de tamaño creciente:
Usuarios
10
100
1.000
10.000


5.2 Uso de memoria

Se utilizó Valgrind (Massif) para medir el uso máximo de heap y detectar fugas de memoria:

valgrind --leak-check=full --tool=massif \
./vibenodes -lista -user data/

valgrind --leak-check=full --tool=massif \
./vibenodes -matriz -admin data/

En todas las ejecuciones se verificó la ausencia de fugas de memoria (0 bytes lost).