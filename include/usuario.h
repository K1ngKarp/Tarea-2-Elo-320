#ifndef USUARIO_H
#define USUARIO_H

#include "hash.h"
#include "utils.h"

char *LlenarTabla(TABLE_HASH *TH, char *usersArch);

char *generarSalt();

char *Registrar(TABLE_HASH *TH,char *archrespaldo);


#endif