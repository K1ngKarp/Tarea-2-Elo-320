#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

TABLE_HASH *crear(int tamano) {
    if (tamano <= 0) {
        printf("Error: Tamaño de tabla inválido.\n");
        return NULL;
    }

    TABLE_HASH *TH = (TABLE_HASH *)malloc(sizeof(TABLE_HASH));
    if (TH == NULL) {
        printf("Error: No se pudo asignar memoria para TABLE_HASH.\n");
        return NULL;
    }

    TH->cantidadUsers = 0;
    TH->capacidad = tamano;

    TH->tabla = (Usuario**)calloc(tamano, sizeof(Usuario*));
    if (TH->tabla == NULL) {
        printf("Error: No se pudo asignar memoria para la tabla de usuarios.\n");
        free(TH);
        return NULL;
    }

    return TH;
}


//funcion hash que grok recomendo pues no supe que funcion podria ser util para este caso.
unsigned long key(char *str){
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;   // hash = hash*33 + c
    }
    return hash;
}

unsigned long h(unsigned long llave,int tamanoTH){
    
    return llave%tamanoTH;

}


void insertarTH(TABLE_HASH *TH, char *nombre,char *pass, char *salt, char *tipo){
   if (TH == NULL || nombre == NULL || pass == NULL || tipo == NULL||salt==NULL) 
        return;

    unsigned long idx = key(nombre);
    
    for (int i = 0; i < TH->capacidad; i++) {
        int index = (idx + i)%TH->capacidad;


        if (TH->tabla[index] == NULL) {
            
            Usuario *nuevo = malloc(sizeof(Usuario));
            
            if (nuevo == NULL) 
                return;

            char *conct=concatenar(salt,pass);
            if(conct==NULL){
                free(nuevo);
                return;
            }

            strncpy(nuevo->nombre, nombre, 99);
            nuevo->nombre[99] = '\0';

            strncpy(nuevo->pass, conct, 99);
            nuevo->pass[99] = '\0';

            strncpy(nuevo->tipo, tipo, 9);
            nuevo->tipo[9] = '\0';

            strncpy(nuevo->salt,salt,5);
            nuevo->salt[5]='\0';
            
            nuevo->clavehash=index;

            nuevo->historialUser=NULL;

            TH->tabla[index]->enuso=1;
            TH->tabla[index] = nuevo;
            TH->cantidadUsers++;
            
            free(conct);
            return;
        }
        // Si ya existe el usuario
        else if (strcmp(TH->tabla[index]->nombre, nombre) == 0) {
            printf("El nombre de usuario ya existe.\n");
            return;
        }
    }
    printf("Tabla hash llena.\n");
}


void LiberarTabla(TABLE_HASH *TH){

    if(TH==NULL) return;

    for (int i = 0; i < TH->capacidad; i++){
        if (TH->tabla[i]!=NULL)    free(TH->tabla[i]);
   
    }
    free(TH->tabla);
    free(TH);
}


unsigned int buscarNomb(TABLE_HASH *TH,char *nombre){

    unsigned long idx = key(nombre);
    unsigned long retorno;

    for (int i = 0; i < TH->capacidad; i++)
    {
        unsigned long index =h(idx+i,TH->capacidad);

        if (TH->tabla[index] != NULL)
        {
            if (strcmp(TH->tabla[index]->nombre, nombre) == 0){
                retorno=index;
                return retorno;
            }

        }
    }
    return TH->capacidad+1; //representa que no se encuentra el nombre.
}
void guardar_usuarios_hashed(TABLE_HASH *TH, char *directorio)
{
    char ruta[256];
    snprintf(ruta, sizeof(ruta), "%susuarios_hashed.csv", directorio);

    FILE *f = fopen(ruta, "w");
    if (f == NULL) return;

    fprintf(f, "# username;password_hash;salt;rol\n");

    for (int i = 0; i < TH->capacidad; i++) {
        if (TH->tabla[i] != NULL && TH->tabla[i]->enuso) {
            fprintf(f, "%s;%s;%s;%s\n",
                    TH->tabla[i]->nombre,
                    TH->tabla[i]->pass,
                    TH->tabla[i]->salt,
                    TH->tabla[i]->tipo);
        }
    }
    fclose(f);
    printf("Archivo usuarios_hashed.csv generado.\n");
}

//TABLE_HASH *TH=CargarTabla(char *ruta);
TABLE_HASH *cargarTH(char *directorio)
{
    char ruta_hashed[256], ruta_plana[256];

    snprintf(ruta_hashed, sizeof(ruta_hashed), "%susuarios_hashed.csv", directorio);
    snprintf(ruta_plana, sizeof(ruta_plana), "%susuarios_100.csv", directorio);

    FILE *f = fopen(ruta_hashed, "r");
    int desde_hashed = (f != NULL);

    if (!desde_hashed) {
        f = fopen(ruta_plana, "r");
        if (f == NULL) {
            printf("Error: no se encontró ningún archivo de usuarios.\n");
            return NULL;
        }
    }

    // Contar usuarios
    int n = 0;
    char linea[300];
    while (fgets(linea, sizeof(linea), f)) {
        if (linea[0] != '#' && linea[0] != '\n')
            n++;
    }
    rewind(f);

    int tam = siguiente_primo(n * 2);
    TABLE_HASH *TH = crear(tam);        
    if (TH == NULL) {
        fclose(f);
        return NULL;
    }

    while (fgets(linea, sizeof(linea), f)) {
        if (linea[0] == '#' || linea[0] == '\n') continue;

        linea[strcspn(linea, "\n")] = '\0';

        char nombre[100];
        char campo2[100];
        char salt[10];
        char tipo[10];

        if (desde_hashed) {
            // username;hash;salt;tipo
            sscanf(linea, "%99[^;];%99[^;];%5[^;];%s", nombre, campo2, salt, tipo);
            salt[4]='\0';
            tipo[strcspn(tipo,"\n")]='\0';
            printf("ayuda1\n");
            insertarTH(TH, nombre, campo2, salt,tipo);
        } 
        else {
            // username;password;salt
            sscanf(linea, "%99[^;];%99[^;];%s", nombre, campo2, salt);
            printf("ayuda2\n");
            insertarTH(TH, nombre, campo2, salt, "user");
        }
    }

    fclose(f);

    if (!desde_hashed) {
        guardar_usuarios_hashed(TH, directorio);
    }

    return TH;
}

void GuardarTabla(TABLE_HASH *TH, char *ruta){
    
    char arch[256];
    snprintf(arch, sizeof(arch), "%susuarios_hashed.csv", ruta);

    FILE *f = fopen(arch, "w");
    if (f == NULL)
    {
        printf("Error al guardar usuarios_hashed.csv\n");
        return;
    }

    fprintf(f, "# username;password_hash;salt;rol\n");

    for (int i = 0; i < TH->capacidad; i++)
    {
        if (TH->tabla[i]->enuso == 1)
        {
            fprintf(f, "%s;%s;%s;%s\n",
                    TH->tabla[i]->nombre,
                    TH->tabla[i]->pass,
                    TH->tabla[i]->salt,
                    TH->tabla[i]->tipo);
        }
    }

    fclose(f);
    printf("Usuarios guardados correctamente.\n");
}

