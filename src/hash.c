#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

TABLE_HASH *crear(int tamano) {
    TABLE_HASH *TH = malloc(sizeof(TABLE_HASH));
    
    if (TH==NULL)
        return NULL;

    TH->cantidadUsers=0;
    TH->capacidad=tamano;
    TH->tabla=(Usuario**)calloc(tamano,sizeof(Usuario*));

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

    unsigned int idx = key(nombre);
    
    for (int i = 0; i < TH->capacidad; i++) {
        int index = h(idx + i,TH->capacidad);

        // Si la casilla está vacía
        if (TH->tabla[index] == NULL) {
            Usuario *nuevo = malloc(sizeof(Usuario));
            if (nuevo == NULL) return;

            char *conct=concatenar(salt,pass);
            strncpy(nuevo->nombre, nombre, 99);
            nuevo->nombre[99] = '\0';

            strncpy(nuevo->pass, conct, 99);
            nuevo->pass[99] = '\0';

            strncpy(nuevo->tipo, tipo, 9);
            nuevo->tipo[9] = '\0';

            strncpy(nuevo->salt,salt,5);
            nuevo->salt[5]='\0';
            
            nuevo->historialUser=NULL;

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


//TABLE_HASH *TH=CargarTabla(char *ruta);
TABLE_HASH *CargarTabla(char *ruta){
    char rutaHashed[256];
    char rutaArchivo[256];
    snprintf(rutaHashed, sizeof(rutaHashed), "%susuarios_hashed.csv", ruta);
    snprintf(rutaArchivo, sizeof(rutaArchivo), "%susuarios_100.csv", ruta);

    int flag=1; //para saber si estamos cargando de la base de usuarios o de los guardados
    int largo;
    FILE *archivo = fopen(rutaHashed, "r");

    if (archivo == NULL){
        archivo=fopen(rutaArchivo,"r");
        if (archivo)
        {
            printf("Error: archivo de usuarios.\n");
            return NULL;
        }
        flag=0;
        largo=siguiente_primo((LargoArchivo(rutaArchivo)-1)*3);    
    
    } else{
        largo=siguiente_primo((LargoArchivo(rutaHashed)-1)*3);
    }

    TABLE_HASH *TH=crear(largo);
    if (TH==NULL){
        fclose(archivo);
        return NULL;
    }
    char Buff[360];
    while (fgets(Buff,sizeof(360),archivo)!=NULL){
        if (Buff[0]== '\n' ||Buff[0]=='\0'||Buff[0]=='#'){
            
        }else{
            Buff[strcspn(Buff,"\n")]='\0';
            if (flag==1){
                char nombre[100], hash_pass[100],salt[6],tipo[10];
                int seguro=sscanf(Buff,"%99[^;];%99[^;];%5[^;];%s",nombre,hash_pass,salt,tipo);
                if (seguro<4){
                    continue;
                }
                tipo[strcspn(tipo,"\n")]='\0';

                unsigned long index=key(nombre);
                for (int i = 0; i < largo; i++){
                    unsigned long indice=h(index+1,largo);

                    if (TH->tabla[indice]==NULL){
                        insertarTH(TH,nombre,hash_pass,salt,tipo);

                        break;
                    }
                }

            }else{ //username;password_plano;salt
                
                char nombre[100], pass[100],salt[6],tipo[10];
                sscanf(Buff,"%99[^;];%99[^;];%s",nombre,pass,salt);
                salt[strcspn(salt,"\n")]='\0';

                char *pass_hash=concatenar(salt,pass);

                unsigned long index=key(nombre);
                for (int i = 0; i < largo; i++){
                    unsigned long indice=h(index+1,largo);

                    if (TH->tabla[indice]==NULL){
                        insertarTH(TH,nombre,pass_hash,salt,tipo);
                        break;
                    }
                }
            }
        }
    }
    return TH;
}

void GuardarTabla(TABLE_HASH *TH, char *ruta){
    char rutaHashed[256];
    snprintf(rutaHashed, sizeof(rutaHashed), "%susuarios_hashed.csv", ruta);
    FILE *arch=fopen(rutaHashed,"w");
    if(arch==NULL) return;

    fprintf(arch,"# username;password_hash;salt;rol");
    for(int i=0;i<TH->capacidad;i++){
        if (TH->tabla[i]==NULL){
            fprintf(arch,"%s;%s;%s;%s\n",TH->tabla[i]->nombre,TH->tabla[i]->pass,TH->tabla[i]->salt,TH->tabla[i]->tipo);
        }
    }

    fclose(arch);
}

