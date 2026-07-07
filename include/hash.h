#ifndef HASH_H
#define HASH_H

#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct Historial
{
    Song *cancion;
    char timestamp[30];
    struct Historial *siguiente;
} Historial;


typedef struct User {
    
    char nombre[100];    
    char pass[100];
    char salt[6];
    char tipo[10]; //user o admin

    Historial *historialUser;

}Usuario;


//tabla hash
typedef struct{
    Usuario **tabla;
    unsigned int capacidad; 
    unsigned int cantidadUsers;
}TABLE_HASH;
//

TABLE_HASH *crear(int tamano);

unsigned long key(char *llave);

unsigned long h(unsigned long key,int tamanoTH);

void insertarTH(TABLE_HASH *TH, char *nombre,char *pass, char *salt,char *tipo);

void LiberarTabla(TABLE_HASH *TH);

unsigned int buscarNomb(TABLE_HASH *TH,char *nombre);

TABLE_HASH *CargarTabla(char *ruta);

void GuardarTabla(TABLE_HASH *TH, char *ruta);

#endif