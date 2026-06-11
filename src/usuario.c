#include "usuario.h"

void LlenarTabla(TABLE_HASH *TH, char *usersArch){
    if (TH==NULL)
    {
        printf("Tabla vacia\n");
        return ;
    }
    
    
    
    FILE *arch=fopen(usersArch,"r");

    if (arch==NULL)
    {
        printf("archivo vacio\n");
        return;
    }
    int largo=LargoArchivo(usersArch);
    
    char nuevoarch[50];
    snprintf(nuevoarch,50,"usuarios_hashed_%d.csv",(largo-1));
    strcpy(TH->respaldo,nuevoarch);

    FILE *respaldo=fopen(nuevoarch,"w");


    char buff[256];
    
    fgets(buff,sizeof(buff),arch);
    
    
    while (fgets(buff,sizeof(buff),arch)!=NULL){


        if (buff[0]=='\0'||buff[0]=='\n'){

        }else{
        
            char *nombre=strtok(buff,";");
            char *pass=strtok(NULL,";");
            char *salt=strtok(NULL,";");
            
            salt[strcspn(salt, "\n")] = '\0';
            

            insertarTH(TH,nombre, pass, salt);

            char *concat=concatenar(salt,pass);

            fprintf(respaldo,"%s;%s;%s\n",nombre,concat,salt);
            
            free(concat);
        }
    }
    fclose(respaldo);
    fclose(arch);

    return ;
}

char *generarSalt(){
    char *num="0123456789";

    char *salt=malloc(sizeof(char)*5);

    srand(time(NULL));
    salt[0]=num[(rand())%(strlen(num))];

    for (int i = 1; i < 4; i++)
        salt[i]=num[(rand())%(strlen(num))];
    salt[4]='\0';
    
    return salt;

}


void Registrar(TABLE_HASH *TH,char *archrespaldo,char *nombre,char *pass){
    char *salt=generarSalt();

    insertarTH(TH,nombre,pass,salt);

    char *a=concatenar(salt,pass);
    
    FILE *nuevo=fopen(archrespaldo,"a");

    fprintf(nuevo,"%s;%s;%s\n",nombre,a,salt);
    
    fclose(nuevo);
    free(a);
    free(salt);
    
    char buff[60];
    int cantact=LargoArchivo(archrespaldo);
    sprintf(buff,"usuarios_hashed_%d.csv",cantact-1);
    rename(archrespaldo,buff);
    strcpy(TH->respaldo,buff);
}



Usuario *login(TABLE_HASH *TH, char *nombre,char *pass,char *arch){
    FILE *respaldo=fopen(arch,"r");
    
    if(respaldo==NULL)  return NULL;
    int index=buscarNomb(TH,nombre);
    
    char *salt;

    char buff[256];
    
    fgets(buff,sizeof(buff),respaldo);
    
    
    while (fgets(buff,sizeof(buff),respaldo)!=NULL){


        if (buff[0]=='\0'||buff[0]=='\n'){

        }else{
        
            strtok(buff,";");
            strtok(NULL,";");
            salt=strtok(NULL,";");
            salt[strcspn(salt, "\n")] = '\0';            

            if (strcmp(TH->tabla[index].cabeza->nombre,nombre)==0){        
                if (strcmp(salt,TH->tabla[index].cabeza->salt)==0){
                    char *concat=concatenar(salt,pass);

                    if(strcmp(TH->tabla[index].cabeza->pass,concat)==0){
                        free(concat);
                        return TH->tabla[index].cabeza;
                    }else{
                        free(concat);
                        printf("Usuario o contraseña incorrecta\n");
                        return NULL;
                    }
                }
                
            }else if(strcmp(TH->tabla[index].cabeza->sgte->nombre,nombre)==0){
                if (strcmp(salt,TH->tabla[index].cabeza->sgte->salt)==0){
                    char *concat=concatenar(salt,pass);

                    if(strcmp(TH->tabla[index].cabeza->sgte->pass,concat)==0){
                        free(concat);
                        return TH->tabla[index].cabeza;
                    }else{
                        free(concat);
                        printf("Usuario o contraseña incorrecta\n");
                        return NULL;
                    }
                }
            }
        }
    }
    
    printf("usuario no encontrado\n");
    fclose(respaldo);
    return NULL;
}

