#ifndef REPRODUCTOR_H
#define REPRODUCTOR_H

#include "avl.h"
#include "hash.h"
#include "grafo.h"

void reproducirCancion(Usuario *usuario, Artista *artista, Song *cancion);
void CargarReproductor(TABLE_HASH *TH, NodoArb *raiz, const char *direct);
void GuardarReproductor(TABLE_HASH *TH, char *directorio);
int Reproducciones(NodoArb *raiz, const char *track_id);

#endif
