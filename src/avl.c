#include "avl.h"

Artistas *crearArtista(char *art){
    
    Artistas *nuevo=malloc(sizeof(Artistas));
    
    if(nuevo==NULL) return NULL;
    
    strcpy(nuevo->NombreArt,art);
    nuevo->derecha=NULL;
    nuevo->izquierda=NULL;
    nuevo->discs=NULL;
    nuevo->slotsdisc=0;
    nuevo->cantidaddiscos=0;
    nuevo->altura=1;

    return nuevo;
}


void insertSong(Discos *disc, char *cancion, char *popularidad, char *duracion, char *Id) {

    if (disc->cancion==NULL) {
        disc->slots = 20;
        disc->cantidadcanciones = 0;
        disc->cancion = malloc(sizeof(Song) * disc->slots);

        for (int i = 0; i < disc->slots; i++){
            disc->cancion[i].cancion_name[0] = '\0';
        }
    }

    for (int i = 0; i < disc->slots; i++) {

        if (strcmp(disc->cancion[i].cancion_name, cancion) == 0) {
            disc->cancion[i].popularidad = atoi(popularidad);
            disc->cancion[i].duracion = atoi(duracion);
            strcpy(disc->cancion[i].Id, Id);
            return;
        }

        if (disc->cancion[i].cancion_name[0] == '\0') {
            
            strcpy(disc->cancion[i].cancion_name, cancion);
            disc->cancion[i].popularidad = atoi(popularidad);
            disc->cancion[i].duracion = atoi(duracion);
            strcpy(disc->cancion[i].Id, Id);
            disc->cantidadcanciones++;
            
            return;
        }
    }
}


void insertardiscos(Artistas *art, char *album, char *cancion, char *popularidad, char *duracion, char *Id) {

    if (art->discs==NULL) {
        art->slotsdisc = 10;
        art->cantidaddiscos = 0;
        art->discs = malloc(sizeof(Discos) * art->slotsdisc);

        for (int i = 0; i < art->slotsdisc; i++){
            art->discs[i].album_name[0] = '\0';
        }
    }

    for (int i = 0; i < art->slotsdisc; i++) {

        if (art->discs[i].album_name[0] == '\0') {
            strcpy(art->discs[i].album_name, album);
            art->discs[i].cancion = NULL;
            art->discs[i].cantidadcanciones = 0;
            art->cantidaddiscos++;

            insertSong(&art->discs[i], cancion, popularidad, duracion, Id);
            return;
        }

        if (strcmp(art->discs[i].album_name, album) == 0) {
            insertSong(&art->discs[i], cancion, popularidad, duracion, Id);
            return;
        }
    }
}

Artistas *buscarArtista(Artistas *raiz, char *nombre){
    if(raiz==NULL)      return NULL;
    
    int comparar= ordenalf(nombre,raiz->NombreArt);

    if (comparar==0)    return raiz;  
    
    if (comparar<0)     return buscarArtista(raiz->izquierda,nombre);

    return buscarArtista(raiz->derecha,nombre);

}


int altura(Artistas *arbol){ //sacado de chatGPT
    if(arbol==NULL) return 0;

    int altIz=altura(arbol->izquierda);
    int altDer=altura(arbol->derecha);
    
    return 1+(altIz > altDer? altIz:altDer);
}

int equilibrio(Artistas *raiz){
    int derecha=altura(raiz->derecha);
    int izquierda=altura(raiz->izquierda);
    
    return izquierda-derecha;
}

Artistas *RotarDer(Artistas *raiz)
{
    Artistas *raizIzq = raiz->izquierda;
    Artistas *T2 = raizIzq->derecha;

    raizIzq->derecha = raiz;
    raiz->izquierda = T2;

    raiz->altura = intmax(altura(raiz->izquierda), altura(raiz->derecha)) + 1;
    raizIzq->altura = intmax(altura(raizIzq->izquierda), altura(raizIzq->derecha)) + 1;

    return raizIzq;
}
Artistas *RotarIz(Artistas *raiz)
{
    Artistas *raizDer = raiz->derecha;
    Artistas *T2 = raizDer->izquierda;

    raizDer->izquierda = raiz;
    raiz->derecha = T2;

    raiz->altura = intmax(altura(raiz->izquierda), altura(raiz->derecha)) + 1;
    raizDer->altura = intmax(altura(raizDer->izquierda), altura(raizDer->derecha)) + 1;

    return raizDer;
}

Artistas *insertArt(Artistas *art, char *nombre){

    if (art == NULL) return crearArtista(nombre);

    if (ordenalf(nombre, art->NombreArt)<0){
        art->izquierda  = insertArt(art->izquierda, nombre);

    }else if (ordenalf(nombre, art->NombreArt)>0){
        art->derecha = insertArt(art->derecha, nombre);

    }else{
        return art;
    }
    art->altura = 1 + intmax(altura(art->izquierda), altura(art->derecha));

    int balance = equilibrio(art);


    // 4 casos

    // izquierda izquierda Case
    if (balance > 1 && ordenalf(nombre,art->NombreArt)<0)
        return RotarDer(art);

    // derecha derecha Case
    if (balance < -1 && ordenalf(nombre,art->NombreArt)>0)
        return RotarIz(art);

    // izquierda derecha Case
    if (balance > 1 && ordenalf(nombre,art->NombreArt)>0)
    {
        art->izquierda =  RotarIz(art->izquierda);
        return RotarDer(art);
    }

    // derecha izquierda Case
    if (balance < -1 && ordenalf(nombre,art->NombreArt)<0)
    {
        art->derecha = RotarDer(art->derecha);
        return RotarIz(art);
    }

    /* return the (unchanged) art pointer */
    return art;
}

void BorrarAVL(Artistas *raiz) {
    if (raiz==NULL) return;

    BorrarAVL(raiz->izquierda);
    BorrarAVL(raiz->derecha);

    for (int i = 0; i < raiz->slotsdisc; i++) {
        if (raiz->discs[i].cancion!=NULL)   free(raiz->discs[i].cancion);
        
    }

    free(raiz->discs);
    free(raiz);
}

void iprimirAVL(Artistas *art){
    
    printf("%s",art->NombreArt);
    
    if (art->izquierda!=NULL)
    {
        iprimirAVL(art->izquierda);
    }
    if (art->derecha!=NULL)
    {
        iprimirAVL(art->derecha);
    }
    
    
}

Artistas *llenarArb(char *archivo) {
    FILE *arch = fopen(archivo, "r");
    if (arch==NULL) return NULL;

    Artistas *raiz = NULL;
    char buffer[256];

    fgets(buffer, sizeof(buffer), arch);

    while (fgets(buffer, sizeof(buffer), arch)) {

        char *art = strtok(buffer, ";");
        char *disc = strtok(NULL, ";");
        char *song = strtok(NULL, ";");
        char *pop = strtok(NULL, ";");
        char *dur = strtok(NULL, ";");
        char *id = strtok(NULL, ";\n");

        raiz = insertArt(raiz, art);
        Artistas *nodo = buscarArtista(raiz, art);

        insertardiscos(nodo, disc, song, pop, dur, id);
    }

    fclose(arch);
    return raiz;
}