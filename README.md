VibeNodes

ELO-320 – Estructuras de Datos y Algoritmos
Tarea 2 Matías Tapia

1. Descripción General

VibeNodes es un sistema de gestión y análisis de datos musicales que integra múltiples estructuras de datos vistas en el curso.
El sistema cuenta con:
- **Tabla Hash** con salted hashing para la gestión de usuarios.
- **Árbol AVL** para el catálogo de artistas con jerarquía de discos y canciones.
- **Grafo de similitud** (Matriz de Adyacencia y Lista de Adyacencia) entre canciones.
- Sistema de recomendaciones mediante **BFS**.
- Persistencia de datos en archivos CSV.
- Historial de reproducciones y estadísticas.

---

## 2. Instrucciones de Compilación y Ejecución

### Compilación
```bash
make          # Compila el proyecto
make clean    # Limpia archivos objeto y ejecutable

Formato general del programa:

./vibenodes <modo_lista> <modo_usuario> <directorio_datos>
Parámetros
<modo_grafo>
-lista: grafo implementado mediante lista de adyacencia.
<modo_usuario>
-user: inicia en modo usuario estándar (login/registro).
<directorio_datos>
Ruta al directorio que contiene los archivos CSV:
usuarios_x.csv
catalogo_x.csv
spotify_tracks_raw.csv
reproducciones_x.csv

Nota: para se tienen prefijados los archivos que se ejecutaran, pero si desea cambiarlo puede ir a las funciones CargarTabla, CargarGrafo, CargarReproductor para cambiar manualmente el archivo al que quiera probar.

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

3.3 Construcción del Grafo
Temporal O(N^2)
Espacial O(V)

3.4 BFS (recomendado por IA Grok)
Temporal O(V+E)
Espacial O(V)

3.5 Heavy Hitters
Temporal O(V+E)
Espacial O(V)


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