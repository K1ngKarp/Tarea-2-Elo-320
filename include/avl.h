#ifndef AVL_H

#define AVL_H

#include "utils.h"

typedef struct
{
    char cancion_name[256];
    int popularidad;
    int duracion;
    char Id[256];

}Song;

typedef struct album
{
    char album_name[60];
    int visitas;
    int cantidadcanciones;
    int slots;   
    
    //lista de canciones
    Song *cancion;
}Discos;


typedef struct AVL{

    char NombreArt[150];
    int altura;
    int cantidaddiscos;
    int slotsdisc;
    
    Discos *discs;

    struct AVL *derecha;
    struct AVL *izquierda;
}Artistas;

Artistas *crearArtista(char *art);
void insertSong(Discos *disc,char *cancion,char *popularidad,char *duracion,char *Id);

void insertardiscos(Artistas *art,char *disc,char *cancion,char *popularidad,char *duracion,char *Id);

Artistas *buscarArtista(Artistas *raiz, char *nombre);

int altura(Artistas *arbol);

int equilibrio(Artistas *raiz);

Artistas *RotarDer(Artistas *raiz);

Artistas *RotarIz(Artistas *raiz);

Artistas *insertArt(Artistas *art, char *nombre);

void BorrarAVL(Artistas *raiz);

Artistas *llenarArb(char *catalogo);


#endif