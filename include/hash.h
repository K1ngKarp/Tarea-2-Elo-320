#ifndef HASH_H
#define HASH_H

#include "utils.h"


#define LARGE_KEY 12011

typedef struct User {
    char nombre[120];    
    char pass[100];
    char salt[20];

    //sera un hashing abierto, pero sertan maximo 2 nodos por lista para evitar complejidad
    struct User *sgte;
}Usuario;

typedef struct{
    Usuario *cabeza;
    int cont;
}slot;


//tabla hash
typedef struct{
    slot *tabla;
    unsigned int capacidad; 
    unsigned int cantidadUsers;
}TABLE_HASH;
//

TABLE_HASH *crear(int tamano);

unsigned int h(int key,int tamanoTH);

unsigned int SlotLibre(TABLE_HASH *TH,int indx);

void insertarTH(TABLE_HASH *TH, char *nombre,char *pass, char *salt);

void LiberarTabla(TABLE_HASH *TH);

void imprimirTH(TABLE_HASH *TH);

// void imprimirColision(TABLE_HASH *TH);

#endif