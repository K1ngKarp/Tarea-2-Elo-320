#include "utils.h"

char *concatenar(char *char1, char *char2){
    
    int largo= strlen(char1)+strlen(char2)+1;
    char *concat=malloc(sizeof(char)*largo);
   
    concat[0]='\0';
    strcat(concat,char1);
    strcat(concat,char2);

    return concat;
}

int LargoArchivo(char *archlec){
    FILE *arch=fopen(archlec,"r");
    if(arch==NULL){
        printf("archibo dañado");
        return -1;
    }
    static char BUFFER[256];
    int large=0;

    while (fgets(BUFFER,sizeof(BUFFER),arch)!=NULL){
        large++;
    }
    
    fclose(arch);

    return large;
}

int esprimo(int numero){
    
    if(numero<2) return 0;
    if(numero<4) return 1;
    if(numero%2==0) return 0;
    
    int raiz=(int)sqrt((double)numero);
    for(int i=3;i< raiz+1;i+=2){
        if(numero%i==0) return 0;
    }
    return 1;
}

int primoMayor(int numero){
    int siguiente=numero+1;
    while (esprimo(siguiente)){
        siguiente++;
    }
    
    return siguiente;
    
}

int intmax(int primero,int segundo){
    if (primero>segundo){
        return primero;
    }else{
        return segundo;
    }    
}

//-1 si el primer string es menor alfabeticamente , 0 si son iguales y 1 el primer string es mayor alfabeticamente
int ordenalf(char *primer,char *segundo){
    if (strlen(primer)!=strlen(segundo)) return (strlen(primer)<strlen(segundo))? -1: 1;
    
    for (int i = 0; i < strlen(primer); i++){
        if (primer[i]!=segundo[i]) return (primer[i]<segundo[i]) ? -1:1;
        
    }
    return 0;
}
