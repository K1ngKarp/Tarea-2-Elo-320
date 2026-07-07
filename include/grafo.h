#ifndef GRAFO_H
#define GRAFO_H

#include "avl.h"
#include "hash.h"

typedef struct NodoVecino
{
    int indice_cancion;
    float peso;
    struct NodoVecino *siguiente;
} NodoVecino;

typedef struct
{
    char track_id[50];
    char nombre[100];
    char artista[100];
    char genero[50];
    float danceability;
    float energy;
    float acousticness;
    float valence;
    float tempo;
} AtributosCancion;

typedef struct
{
    int N;
    AtributosCancion *atributos;

    NodoVecino **lista;

    float **matriz;

    int i_matriz;
} Grafo;
//

typedef struct
{
    int indice; 
    float peso; 
} Recomendacion;



typedef struct {
    int *datos;
    int  frente;
    int  fondo;
    int  tam;
} Cola;

Grafo *crear_grafo(int N, int i_matriz);
Grafo *cargar_grafo(const char *directorio, int N, int i_matriz, NodoArb *raiz);

void agregar_arista(Grafo *grafo, int i, int j, float peso);
float obtener_peso(Grafo *grafo, int i, int j);
void liberar_grafo(Grafo *grafo);
void bfs_recomendaciones(Grafo *grafo, int idx_origen, Usuario *usuario, Recomendacion *resultado, int *n_resultados);
Cola *crear_cola(int tam);
void  encolar(Cola *cola, int idx);
int   desencolar(Cola *cola);
int   cola_vacia(Cola *cola);
void  liberar_cola(Cola *cola);
void cargar_ids_desde_avl(NodoArb *raiz, AtributosCancion *atributos, int N);
void heavy_hitters(Grafo *grafo, NodoArb *arbol);
int contar_aristas(Grafo *grafo);
void artista_mas_escuchado(TABLE_HASH *TH, Grafo *grafo,char *art, int *max);

#endif