#include "hash.h"
#include "usuario.h"
#include "avl.h"
#include "utils.h"
#include <unistd.h> //exclusivamente para la contraseña secreta

int main(int argc, char *argv[]){
    if (argc!=4){
        printf("Formato incorrecto.\n");
        printf("Formato ideal:\n");
        printf("./vibenodes -lista -user data/\n") ;
        printf("./vibenodes -matriz -admin data/ \n");
        return 1;
    }

    char *modo      =argv[2];
    char *data      =argv[3];

    if(strcmp(modo,"-user")!=0 ||strcmp(modo,"-admin")!=0){
        printf("Solo se permite acceso de -user o -admin, ningun otro modo se permite.\n");
        return -1;
    }
    printf("=== VibeNodes ===\n");
    printf("cargando datos de %s\n", data);
    char ruta[250];
    snprintf(ruta,250,"%susuarios_1000.csv",data);
    
    printf("Cantidad de usuarios registrados?(segun cantidad guardada en %s): ",data);
    
    printf("Cargando Tabla...\n");
    
    

    int tamaño=PrimoMayorQue(LargoArchivo(ruta)*2);
    TABLE_HASH *TH=crear(tamaño);
    LlenarTabla(TH,ruta);

    if (strcmp(modo,"-user")==0){
        printf("=== VibeNodes ===\n");
        printf("1. Iniciar sesíon\n");
        printf("2. Registrarse\n");
        int flag=1;
        char resp[5];
        
        while (flag==1){
            scanf("%2s",resp);
            if ((resp[0]=='1' ||resp[0]=='2')&&resp[1]=='\0'){
                flag=0;
            }
        }
        char nombre[100];
        char *pass;

        if (resp[0]=='1'){
            
            flag=1;
            
            while (flag==1){
            
                printf("Ingrese nombre de usuario: ");
                sncanf("%s",nombre);

                pass=getpass("Ingrese contraseña: ");

                Usuario *user=login(TH,nombre,pass,ruta);
                if (user!=NULL){
                    printf("¡Bienvenido, %s!",nombre);
                    flag=0;
                }
                
            }

        } else{
            flag=1;
            
            while (flag==1){
                
                printf("Ingrese nombre de usuario: ");
                sncanf(" %s",nombre);

                pass= getpass("Ingrese contraseña: ");
                if (buscarNomb(TH,nombre)==NULL){
                    Registrar(TH,TH->respaldo,nombre,pass);
                    printf("Registro exitoso.\n");
                    flag=0;
                } else{
                    printf("usuario ya existente, intente con otro nombre de usuario.\n");
                }
            }
        }
    }
    
    
    LiberarTabla(TH);
    return 0;
}