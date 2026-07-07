#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "reproductor.h"

void reproducirCancion(Usuario *usuario, Artista *artista, Song *cancion)
{
    Historial *nuevo = malloc(sizeof(Historial));
    if (nuevo == NULL){
        printf("Error al asignar memoria para el historial.\n");
        return;
    }

    cancion->reproducciones++;
    artista->popularidad++;
    
    nuevo->cancion = cancion;

    time_t ahora = time(NULL);
    strftime(nuevo->timestamp, sizeof(nuevo->timestamp), "%Y-%m-%dT%H:%M:%S", localtime(&ahora));

    nuevo->siguiente = usuario->historialUser;
    usuario->historialUser = nuevo;

    printf("Reproduciendo: %s | %s\n", cancion->cancion_name, artista->nombre);
    printf("Reproducciones totales: %d\n", cancion->reproducciones);
}

void CargarReproductor(TABLE_HASH *TH, NodoArb *raiz, const char *direct){
    if (TH == NULL || raiz == NULL) return;

    char ruta[256];
    snprintf(ruta, sizeof(ruta), "%sreproduccciones.csv", direct);

    FILE *f = fopen(ruta, "r");
    if (f == NULL) {
        printf("Archivo de reproducciones no encontrado (primera ejecución).\n");
        return;
    }

    char linea[256];
    int cargadas = 0;

    while (fgets(linea, sizeof(linea), f)){
        if (linea[0] == '#' || linea[0] == '\n') continue;

        linea[strcspn(linea, "\n")] = '\0';

        char username[100], track_id[100], timestamp[30] = {0};

        if (sscanf(linea, "%49[^;];%49[^;];%29s", username, track_id, timestamp) != 3)
            continue;

        // Buscar usuario
        Usuario *usuario = NULL;
        unsigned int idx = key(username);

        for (int i = 0; i < TH->capacidad; i++){
            int pos = (idx + i) % TH->capacidad;

            if (TH->tabla[pos] == NULL) continue;

            if (strcmp(TH->tabla[pos]->nombre, username) == 0){
                usuario = TH->tabla[pos]; 
                break;
            }
        }
        if (usuario == NULL) continue;

        Song *cancion = CancionId(raiz, track_id);
        if (cancion == NULL) continue;

        Historial *entrada = malloc(sizeof(Historial));
        if (entrada == NULL) continue;

        entrada->cancion = cancion;
        strncpy(entrada->timestamp, timestamp, 29);
        entrada->timestamp[29] = '\0';
        entrada->siguiente = usuario->historialUser;
        usuario->historialUser = entrada;

        cancion->reproducciones++;
        cargadas++;
    }

    fclose(f);
}


void GuardarReproductor(TABLE_HASH *TH, char *directorio){
    if (TH == NULL) return;

    char ruta[256];
    snprintf(ruta, sizeof(ruta), "%sreproduccciones.csv", directorio);

    FILE *f = fopen(ruta, "w");
    if (f == NULL){
        printf("Error al guardar reproducciones.csv\n");
        return;
    }

    fprintf(f, "# username;track_id;timestamp\n");

    for (int i = 0; i < TH->capacidad; i++){
        if (TH->tabla[i] == NULL)
            continue;   

        Historial *historial = TH->tabla[i]->historialUser;
        while (historial != NULL)
        {
            if (historial->cancion != NULL){
                fprintf(f, "%s;%s;%s\n",TH->tabla[i]->nombre,historial->cancion->Id,historial->timestamp);
            }
            historial = historial->siguiente;
        }
    }

    fclose(f);
    printf("Reproducciones guardadas correctamente.\n");
}

int Reproducciones(NodoArb *raiz, const char *track_id)
{
    if (raiz == NULL) return 0;

    Disco *disco = raiz->artista->discos;
    while (disco != NULL) {
        Song *cancion = disco->canciones;
        while (cancion != NULL) {
            if (strcmp(cancion->Id, track_id) == 0) {
                if (cancion->reproducciones > 0)
                return cancion->reproducciones;
            }
            cancion = cancion->siguiente;
        }
        disco = disco->siguiente;
    }

    int izq = Reproducciones(raiz->izquierda, track_id);
    if (izq > 0) return izq;
    return Reproducciones(raiz->derecha, track_id);
}
