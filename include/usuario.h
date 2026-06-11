#ifndef USUARIO_H
#define USUARIO_H

#include "hash.h"
#include "utils.h"

void LlenarTabla(TABLE_HASH *TH, char *usersArch);

char *generarSalt();

void Registrar(TABLE_HASH *TH,char *archrespaldo);

Usuario *login(TABLE_HASH *TH, char *nombre,char *pass,char *arch);

#endif