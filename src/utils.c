#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

char *concatenar(char *char1, char *char2){
    
    char *concat=malloc(strlen(char1)+strlen(char2)+1);
    if (concat == NULL) return NULL;

    strcpy(concat,char1);
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

//funcion auxiliar proporcionada por Grok
int siguiente_primo(int n) {
    // Simple: retorna un primo >= n (ajusta según necesidad)
    if (n < 2) return 2;
    int primo = n | 1; // impar
    while (1) {
        int es_primo = 1;
        for (int i = 3; i * i <= primo; i += 2) {
            if (primo % i == 0) { es_primo = 0; break; }
        }
        if (es_primo) return primo;
        primo += 2;
    }
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
    
    for (long unsigned int i = 0; i < strlen(primer); i++){
        if (primer[i]!=segundo[i]) return (primer[i]<segundo[i]) ? -1:1;
        
    }
    return 0;
}

