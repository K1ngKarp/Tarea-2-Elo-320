#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

Grafo *crear_grafo(int N, int i_matriz)
{
    Grafo *g = malloc(sizeof(Grafo));
    g->N = N;
    g->i_matriz = i_matriz;

    // atributos de cada canción
    g->atributos = calloc(N, sizeof(AtributosCancion));

    if (i_matriz)
    {
        // crear matriz N x N 
        g->matriz = malloc(N * sizeof(float *));
        for (int i = 0; i < N; i++)
        {
            g->matriz[i] = calloc(N, sizeof(float));
        }
        g->lista = NULL;
    }
    else
    {

        g->lista = malloc(N * sizeof(NodoVecino *));
        for (int i = 0; i < N; i++)
            g->lista[i] = NULL;
        g->matriz = NULL;
    }

    return g;
}

void agregar_arista(Grafo *g, int i, int j, float peso)
{
    if (g->i_matriz)
    {

        g->matriz[i][j] = peso;
        g->matriz[j][i] = peso;
    }
    else
    {

        NodoVecino *nodo_ij = malloc(sizeof(NodoVecino));
        nodo_ij->indice_cancion = j;
        nodo_ij->peso = peso;
        nodo_ij->siguiente = g->lista[i];
        g->lista[i] = nodo_ij;

        // j → i
        NodoVecino *nodo_ji = malloc(sizeof(NodoVecino));
        nodo_ji->indice_cancion = i;
        nodo_ji->peso = peso;
        nodo_ji->siguiente = g->lista[j];
        g->lista[j] = nodo_ji;
    }
}

float Peso(Grafo *g, int i, int j)
{
    if (g->i_matriz)
    {
        return g->matriz[i][j];
    }
    else
    {

        NodoVecino *actual = g->lista[i];
        while (actual != NULL)
        {
            if (actual->indice_cancion == j)
                return actual->peso;
            actual = actual->siguiente;
        }
        return 0.0f;
    }
}

static int idx_global = 0;

static void _cargar_ids(NodoArb *raiz, AtributosCancion *atributos, int N)
{
    if (raiz == NULL || idx_global >= N)
        return;
    _cargar_ids(raiz->izquierda, atributos, N);
    Disco *disco = raiz->artista->discos;
    while (disco != NULL && idx_global < N)
    {
        Song *cancion = disco->canciones;
        while (cancion != NULL && idx_global < N)
        {
            strncpy(atributos[idx_global].track_id, cancion->Id, 49);
            strncpy(atributos[idx_global].nombre, cancion->cancion_name, 99);
            strncpy(atributos[idx_global].artista, raiz->artista->nombre, 99);
            idx_global++;
            cancion = cancion->siguiente;
        }
        disco = disco->siguiente;
    }
    _cargar_ids(raiz->derecha, atributos, N);
}

void cargar_ids_desde_avl(NodoArb *raiz, AtributosCancion *atributos, int N)
{
    idx_global = 0;
    _cargar_ids(raiz, atributos, N);
}

static float calcular_distancia(AtributosCancion *a, AtributosCancion *b)
{
    float diff_dance = a->danceability - b->danceability;
    float diff_ener = a->energy - b->energy;
    float diff_acous = a->acousticness - b->acousticness;
    float diff_val = a->valence - b->valence;
    float diff_tempo = a->tempo - b->tempo;

    return sqrtf(diff_dance * diff_dance +diff_ener * diff_ener +diff_acous * diff_acous +diff_val * diff_val +diff_tempo * diff_tempo);
}

Grafo *cargar_grafo(const char *directorio, int N, int i_matriz, NodoArb *raiz)
{
    Grafo *g = crear_grafo(N, i_matriz);

    
    cargar_ids_desde_avl(raiz, g->atributos, N);

    
    char ruta_raw[256];
    snprintf(ruta_raw, sizeof(ruta_raw), "%sspotify_tracks_raw.csv", directorio);
    FILE *fraw = fopen(ruta_raw, "r");
    if (fraw == NULL)
    {
        printf("Error: no se encontro spotify_tracks_raw.csv\n");
        return g;
    }

    char linea[1024];
    fgets(linea, sizeof(linea), fraw); // saltar cabecera
    int idx = 0;
    while (fgets(linea, sizeof(linea), fraw))
    {
        linea[strcspn(linea, "\n")] = '\0';

        char track_id[50], artista[100], album[100], nombre[100];
        float popularidad, duracion, explicito;
        float danceability, energy, key, loudness, mode;
        float speechiness, acousticness, instrumentalness;
        float liveness, valence, tempo;
        char genero[50];

        int leidos = sscanf(linea,"%*f,%49[^,],%99[^,],%99[^,],%99[^,],%f,%f,%f,""%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%*f,%49s",track_id, artista, album, nombre,&popularidad, &duracion, &explicito,&danceability, &energy, &key, &loudness, &mode,&speechiness, &acousticness, &instrumentalness,&liveness, &valence, &tempo, genero);

        if (leidos < 18)
            continue;

        for (int k = 0; k < N; k++)
        {
            if (strcmp(g->atributos[k].track_id, track_id) == 0)
            {
                g->atributos[k].danceability = danceability;
                g->atributos[k].energy = energy;
                g->atributos[k].acousticness = acousticness;
                g->atributos[k].valence = valence;
                g->atributos[k].tempo = tempo / 200.0f;
                strncpy(g->atributos[k].artista, artista, 99);
                strncpy(g->atributos[k].genero, genero, 49);
                idx++;
                break;
            }
        }

        if (idx == N)
            break;
    }
    fclose(fraw);

    
    char ruta_grafo[256];
    snprintf(ruta_grafo, sizeof(ruta_grafo), "%sgrafo_similitud.csv", directorio);
    FILE *f = fopen(ruta_grafo, "r");

    if (f != NULL)
    {
        printf("Cargando grafo desde grafo_similitud.csv...\n");
        char linea2[200];
        int aristas_cargadas = 0;
        while (fgets(linea2, sizeof(linea2), f))
        {
            if (linea2[0] == '#' || linea2[0] == '\n')
                continue;
            linea2[strcspn(linea2, "\n")] = '\0';

            char id_origen[50], id_destino[50];
            float peso;
            if (sscanf(linea2, "%49[^;];%49[^;];%f",
                       id_origen, id_destino, &peso) != 3)
                continue;

            int i = -1, j = -1;
            for (int k = 0; k < N; k++)
            {
                if (strcmp(g->atributos[k].track_id, id_origen) == 0)
                    i = k;
                if (strcmp(g->atributos[k].track_id, id_destino) == 0)
                    j = k;
            }
            if (i != -1 && j != -1)
            {
                agregar_arista(g, i, j, peso);
                aristas_cargadas++;
            }
        }
        printf("Grafo cargado: %d aristas.\n", aristas_cargadas);
        fclose(f);
        return g;
    }

    
    int aristas = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            float d = calcular_distancia(&g->atributos[i], &g->atributos[j]);
            float w = 1.0f / (1.0f + d);
            if (w >= 0.5f &&
                (strcmp(g->atributos[i].artista, g->atributos[j].artista) == 0 ||
                 strcmp(g->atributos[i].genero, g->atributos[j].genero) == 0))
            {
                agregar_arista(g, i, j, w);
                aristas++;
            }
        }
    }
    printf("Grafo construido: %d canciones, %d aristas.\n", N, aristas);

    FILE *fg = fopen(ruta_grafo, "w");
    if (fg != NULL)
    {
        fprintf(fg, "# track_id_origen;track_id_destino;peso\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j++)
            {
                float w = Peso(g, i, j);
                if (w > 0.0f)
                    fprintf(fg, "%s;%s;%.3f\n",
                            g->atributos[i].track_id,
                            g->atributos[j].track_id, w);
            }
        }
        fclose(fg);
    }

    return g;
}

static int ya_escuchada(Usuario *usuario, const char *track_id)
{
    Historial *h = usuario->historialUser;
    while (h != NULL)
    {
        if (strcmp(h->cancion->Id, track_id) == 0)
            return 1;
        h = h->siguiente;
    }
    return 0;
}

Cola *crear_cola(int tam)
{
    Cola *cola = malloc(sizeof(Cola));
    cola->datos = malloc(tam * sizeof(int));
    cola->frente = 0;
    cola->fondo = 0;
    cola->tam = tam;
    return cola;
}

void encolar(Cola *cola, int idx)
{
    
    if (cola->fondo >= cola->tam)
    {
        printf("Error: cola llena.\n");
        return;
    }
    cola->datos[cola->fondo++] = idx;
}

int desencolar(Cola *cola)
{

    if (cola_vacia(cola))
    {
        printf("Error: cola vacia.\n");
        return -1;
    }
    return cola->datos[cola->frente++];
}

int cola_vacia(Cola *cola)
{
    return cola->frente == cola->fondo;
}

void liberar_cola(Cola *cola)
{
    free(cola->datos);
    free(cola);
}

void bfs_recomendaciones(Grafo *g, int idx_origen,Usuario *usuario,Recomendacion *resultado,int *n_resultados)
{
    *n_resultados = 0;

    int *visitado = calloc(g->N, sizeof(int));
    if (visitado == NULL)
        return;

    Cola *cola = crear_cola(g->N);

    encolar(cola, idx_origen);
    visitado[idx_origen] = 1;

    while (!cola_vacia(cola) && *n_resultados < 3)
    {

        int actual = desencolar(cola);

        if (g->i_matriz)
        {

            for (int j = 0; j < g->N; j++)
            {
                if (g->matriz[actual][j] > 0.0f && !visitado[j])
                {
                    visitado[j] = 1;

                    if (!ya_escuchada(usuario, g->atributos[j].track_id))
                    {
                        resultado[*n_resultados].indice = j;
                        resultado[*n_resultados].peso = g->matriz[actual][j];
                        (*n_resultados)++;
                        if (*n_resultados == 3)
                            break;
                    }

                    encolar(cola, j);
                }
            }
        }
        else
        {

            NodoVecino *vecino = g->lista[actual];
            while (vecino != NULL && *n_resultados < 3)
            {
                int j = vecino->indice_cancion;

                if (!visitado[j])
                {
                    visitado[j] = 1;

                    
                    if (!ya_escuchada(usuario, g->atributos[j].track_id))
                    {
                        resultado[*n_resultados].indice = j;
                        resultado[*n_resultados].peso = vecino->peso;
                        (*n_resultados)++;
                    }

                    encolar(cola, j);
                }
                vecino = vecino->siguiente;
            }
        }
    }

    liberar_cola(cola);
    free(visitado);
}

void liberar_grafo(Grafo *g)
{
    if (g == NULL)
        return;

    if (g->i_matriz)
    {
        for (int i = 0; i < g->N; i++)
            free(g->matriz[i]);
        free(g->matriz);
    }
    else
    {
        for (int i = 0; i < g->N; i++)
        {
            NodoVecino *actual = g->lista[i];
            while (actual != NULL)
            {
                NodoVecino *tmp = actual;
                actual = actual->siguiente;
                free(tmp);
            }
        }
        free(g->lista);
    }

    free(g->atributos);
    free(g);
}
void heavy_hitters(Grafo *grafo, NodoArb *arbol)
{
    int total = 0;
    for (int i = 0; i < grafo->N; i++)
    {

        int rep = ObtenerReproducciones(arbol, grafo->atributos[i].track_id);
        if (rep > 0)
            total += rep;
    }

    if (total == 0)
    {
        printf("No hay reproducciones registradas.\n");
        return;
    }

    printf("\n=== Reporte Heavy Hitters ===\n");
    printf("Total de reproducciones: %d\n", total);
    printf("Comunidades con mas del 20%% del total (> %d reproducciones):\n\n",
           (int)(total * 0.2f));

    int *visitado = calloc(grafo->N, sizeof(int));
    int num_comunidad = 1;

    for (int i = 0; i < grafo->N; i++)
    {
        if (visitado[i])
            continue;

        Cola *cola = crear_cola(grafo->N);
        int *comunidad = malloc(grafo->N * sizeof(int));
        int tam_comunidad = 0;
        int suma = 0;

        encolar(cola, i);
        visitado[i] = 1;

        while (!cola_vacia(cola))
        {
            int actual = desencolar(cola);
            comunidad[tam_comunidad++] = actual;
            suma += ObtenerReproducciones(arbol, grafo->atributos[actual].track_id);

            if (grafo->i_matriz)
            {
                for (int j = 0; j < grafo->N; j++)
                    if (grafo->matriz[actual][j] > 0.0f && !visitado[j])
                    {
                        visitado[j] = 1;
                        encolar(cola, j);
                    }
            }
            else
            {
                NodoVecino *vecino = grafo->lista[actual];
                while (vecino != NULL)
                {
                    if (!visitado[vecino->indice_cancion])
                    {
                        visitado[vecino->indice_cancion] = 1;
                        encolar(cola, vecino->indice_cancion);
                    }
                    vecino = vecino->siguiente;
                }
            }
        }

        float porcentaje = (float)suma / total * 100.0f;
        if (porcentaje > 20.0f)
        {
            printf("Comunidad %d:\n", num_comunidad++);
            for (int k = 0; k < tam_comunidad; k++)
            {
                int idx = comunidad[k];
                int rep = ObtenerReproducciones(arbol, grafo->atributos[idx].track_id);
                printf("  - %s (%s) : %d repr.\n",
                       grafo->atributos[idx].nombre,
                       grafo->atributos[idx].artista,
                       rep);
            }
            printf("  Subtotal: %d reproducciones (%.1f%%)\n\n", suma, porcentaje);
        }

        liberar_cola(cola);
        free(comunidad);
    }

    free(visitado);
}

int contar_aristas(Grafo *g)
{
    int total = 0;
    if (g->i_matriz)
    {
        for (int i = 0; i < g->N; i++)
            for (int j = i + 1; j < g->N; j++)
                if (g->matriz[i][j] > 0.0f)
                    total++;
    }
    else
    {
        for (int i = 0; i < g->N; i++)
        {
            NodoVecino *v = g->lista[i];
            while (v != NULL)
            {
                total++;
                v = v->siguiente;
            }
        }
        total /= 2;
    }
    return total;
}

void artista_mas_escuchado(TABLE_HASH *TH, Grafo *grafo, char *art, int *max)
{
    *max = 0;
    art[0] = '\0';

    for (int i = 0; i < grafo->N; i++)
    {
        // evitar contar el mismo artista varias veces (si tiene varias canciones en el grafo)
        int ya_visto = 0;
        for (int j = 0; j < i; j++)
        {
            if (strcmp(grafo->atributos[j].artista, grafo->atributos[i].artista) == 0)
            {
                ya_visto = 1;
                break;
            }
        }
        if (ya_visto)
            continue;

        int count = 0;
        for (int i = 0; i < TH->capacidad; i++)
        {
            if (!TH->tabla[i])
                continue;
            Historial *historial = TH->tabla[i]->historialUser;
            while (historial != NULL)
            {
                for (int k = 0; k < grafo->N; k++)
                {
                    if (strcmp(grafo->atributos[k].track_id, historial->cancion->Id) == 0 &&
                        strcmp(grafo->atributos[k].artista, grafo->atributos[i].artista) == 0)
                    {
                        count++;
                        break;
                    }
                }
                historial = historial->siguiente;
            }
        }

        if (count > *max)
        {
            *max = count;
            strncpy(art, grafo->atributos[i].artista, 99);
            grafo->atributos[i].artista[99]='\0';
        }
    }
}
