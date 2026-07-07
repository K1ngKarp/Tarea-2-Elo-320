#ifndef AVL_H

#define AVL_H

#include "utils.h"

typedef struct Song
{
    char cancion_name[256];
    char album_name[256];
    int popularidad;
    int duracion_ms;
    char Id[256];
    unsigned int reproducciones;
    struct Song *siguiente;
    struct Song *anterior;

}Song;

typedef struct Disco {
    char nombre_disco[256];
    Song* canciones;
    struct Disco *siguiente;
    struct Disco *anterior;
} Disco;

typedef struct Artista {
    char nombre[100];
    Disco* discos;
    int popularidad;
} Artista;

typedef struct AVL{

    Artista *artista;
    struct AVL *derecha;
    struct AVL *izquierda;

    int altura;
}NodoArb;

int altura(NodoArb *arbol);
void actualizarAltura(NodoArb *nodo);
int equilibrio(NodoArb *raiz);

NodoArb *RotarDer(NodoArb *y);
NodoArb *RotarIz(NodoArb *x);


NodoArb *crearAVL(void);

Song *crearCancion( char* nombre,  char* id, int popularidad, long duracion_ms);

void BorrarCancion(Song *cancion);

Disco *CrearDisco(char *nombre);

void insertCancion(Disco *disc, Song *cancion);

Song *BuscarCancion(Disco *disc, char *cancion);

void BorrarDisco(Disco *disc);

void mostrarDiscografia(Artista *artista);

Artista *CrearArtista(char *nombre);


void insertardiscos(Artista *art, Disco *album) ;

Disco *BuscarDisco(Artista *art, char *disc);
Artista *BuscarArtista(NodoArb *raiz, char *artista);

NodoArb*InsertArtista(NodoArb *raiz, Artista *artista);

//Arboles

void BorrarArbol(NodoArb *raiz);

NodoArb *cargarDatos(const char *directorio);
#endif