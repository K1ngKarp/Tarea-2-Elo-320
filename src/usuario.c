#include "usuario.h"

char *LlenarTabla(TABLE_HASH *TH, char *usersArch){
    if (TH==NULL)
    {
        printf("Tabla vacia\n");
        return NULL;
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

    FILE *respaldo=fopen(nuevoarch,"w");


    char buff[256];
    
    fgets(buff,sizeof(buff),arch);

    //fprintf(respaldo, "#username;password_hash;salt\n");
    
    
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

    return nuevoarch;
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


char *Registrar(TABLE_HASH *TH,char *archrespaldo){
    char nombre[102];
    char pass[102];
    printf("Ingrese Nombre de Usuario (maximo 99 caracteres): ");
    scanf("%s",nombre);

    printf("Ingrese Contraseña (maximo 99 caracteres): ");
    scanf("%s",pass);

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

    return buff;
}



Usuario *login(TABLE_HASH *TH, char *nombre,char *pass_hash,char *salt){
    unsigned int hash=h(key(pass_hash),TH->capacidad);
    int retorno;
    
    
    
    for (int i = 0; i < TH->capacidad; i++){
        
        if(i=0){
            retorno=hash;
        }else{
            retorno=(hash+(i*i))%TH->capacidad;
        }
        
        if (strcmp(TH->tabla[i].cabeza->nombre,nombre)==0 && strcmp(TH->tabla[i].cabeza->pass,pass_hash+4)==0 && strcmp(TH->tabla[i].cabeza->salt,salt)==0){
            
            return retorno;

        }else if (TH->tabla[i].cabeza->sgte!=NULL){
            
        }
            
    }
    return NULL;
}