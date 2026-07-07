#ifndef USUARIO_H
#define USUARIO_H

#include "hash.h"
#include "avl.h"
#include "utils.h"
#include "grafo.h"
#include "reproductor.h"
#include <unistd.h> //exclusivamente para la contraseña secreta


char *generarSalt();

Usuario *login(TABLE_HASH *TH, char *nombre,char *pass);

int Registrar(TABLE_HASH *TH,char *nombre,char *pass);

void MenuUser(Usuario *usuario, NodoArb *raiz, Grafo *grafo);
void MenuAdmin(Usuario *admin, TABLE_HASH *TH, NodoArb *raiz, Grafo *grafo);
#endif