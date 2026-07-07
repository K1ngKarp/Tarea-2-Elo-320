#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int altura(NodoArb *arbol){ //sacado de chatGPT
    if(arbol==NULL) return 0;

    int altIz=altura(arbol->izquierda);
    int altDer=altura(arbol->derecha);
    
    return 1+(altIz > altDer? altIz:altDer);
}

int equilibrio(NodoArb *raiz){
    int derecha=altura(raiz->derecha);
    int izquierda=altura(raiz->izquierda);
    
    return izquierda-derecha;
}

void actualizarAltura(NodoArb *nodo)
{
    int h_izquierda = altura(nodo->izquierda);
    int h_derecha = altura(nodo->derecha);
    nodo->altura = 1 + (h_izquierda > h_derecha ? h_izquierda : h_derecha);
}

NodoArb *RotarDer(NodoArb *raiz)
{
    NodoArb *Iz = raiz->izquierda;
    NodoArb * a2= Iz->derecha;

    
    Iz->derecha = raiz;
    raiz->izquierda = a2;


    actualizarAltura(raiz);
    actualizarAltura(Iz);

    
    return Iz;
}

NodoArb *RotarIz(NodoArb *raiz)
{
    NodoArb *Der = raiz->derecha;
    NodoArb *a2 = Der->izquierda;

    
    Der->izquierda = raiz;
    raiz->derecha = a2;

    
    actualizarAltura(raiz);
    actualizarAltura(Der);

    
    return Der;
}

NodoArb *crearNodoAVL(Artista *art){
    
    NodoArb* arbol = (NodoArb*)malloc(sizeof(NodoArb));
    arbol->artista=art;
    arbol->derecha = NULL;
    arbol->izquierda = NULL;
    arbol->altura = 0;
    return arbol;
   
}

Song *crearCancion( char* nombre,  char* id, int popularidad, long duracion_ms){
    Song *cancion=(Song*)malloc(sizeof(Song));
    if (cancion==NULL)
    {
        printf("error al guardar memoria de Song.\n");
        return NULL;
    }
    strncpy(cancion->cancion_name,nombre,254);
    cancion->cancion_name[255]='\0';
    strncpy(cancion->Id,id,254);
    cancion->popularidad = popularidad;
    cancion->duracion_ms = duracion_ms;
    cancion->reproducciones = 0;
    cancion->siguiente = NULL;
    cancion->anterior=NULL;
    return cancion;
}
void BorrarCancion(Song *cancion){
    if (cancion==NULL) return;
    free(cancion);

}


Disco *CrearDisco(char *nombre){
    Disco* disco = (Disco*)malloc(sizeof(Disco));
    if (disco == NULL) return NULL;

    strncpy(disco->nombre_disco,nombre,254);
    disco->nombre_disco[255]='\0';
    disco->canciones = NULL;
    disco->siguiente = NULL;
    disco->anterior = NULL;

    return disco;
}

//cancion previamente cargada, se inserta al inicio
void insertCancion(Disco *disc, Song *cancion) {

    if (disc==NULL||cancion==NULL)
    {
        printf("No se encontro disco o cancion al insertar cancion\n");
        return;
    }
    
    cancion->siguiente=disc->canciones;
    if (disc->canciones!=NULL)  disc->canciones->anterior=cancion;
    
    disc->canciones=cancion;
    cancion->anterior=NULL;
    
}

Song *BuscarCancion(Disco *disc, char *cancion){
    if(disc==NULL) return NULL;
    Song *actual=disc->canciones;

    while (actual!=NULL)
    {
        if (strcmp(actual->cancion_name, cancion) == 0)
            return actual;
        actual = actual->siguiente;
    }
    return NULL;
}

void BorrarDisco(Disco *disc){
    if (disc == NULL) return;

    Song* actual = disc->canciones;
    Song* temp;
    while (actual!=NULL){
        temp = actual;
        actual = actual->siguiente;
        BorrarCancion(temp);
    }
    free(disc);
}

void mostrarDiscografia(Artista *artista)
{
    if (artista == NULL)
    {
        printf("Artista no encontrado.\n");
        return;
    }

    printf("\nArtista: %s | Popularidad: %d\n",artista->nombre, artista->popularidad);

    Disco *disco = artista->discos;
    int numDisco = 1;

    while (disco != NULL)
    {
        printf("\n  Disco %d: %s\n", numDisco, disco->nombre_disco);

        Song *cancion = disco->canciones;
        int numCancion = 1;

        while (cancion != NULL)
        {
            printf("    %d. %s [Popularidad: %d | Duracion: %d ms | Reproducciones: %d]\n",
                   numCancion,cancion->cancion_name,cancion->popularidad,cancion->duracion_ms,cancion->reproducciones);

            cancion = cancion->siguiente;
            numCancion++;
        }

        disco = disco->siguiente;
        numDisco++;
    }
    printf("\n");
}

Artista *CrearArtista(char *nombre){
    Artista* art = (Artista*)malloc(sizeof(Artista));
    if (art == NULL) return NULL;

    strncpy(art->nombre,nombre,98);
    art->nombre[99]='\0';

    art->discos=NULL;
    art->popularidad=0;
    return art;
}

void insertardiscos(Artista *art, Disco *album) {

    if (art==NULL||album==NULL)
    {
        printf("No se encontro artista o disco al insertar disco\n");
        return;
    }
    if (art->discos!=NULL){
        art->discos->anterior=album;
    }
    
    album->siguiente=art->discos;
    art->discos=album;

}

Disco *BuscarDisco(Artista *art, char *disc){
    if (art == NULL) return NULL;

    Disco* actual = art->discos;
    while (actual!=NULL){
        if (strcmp(actual->nombre_disco,disc) == 0) return actual;

        actual = actual->siguiente;
    }
    return NULL;
}

Artista *BuscarArtista(NodoArb *raiz, char *artista){
    if (raiz==NULL) return NULL;
    
    if (ordenalf(artista, raiz->artista->nombre) < 0)
    {
        return BuscarArtista(raiz->izquierda, artista);
    }
    else if (ordenalf(artista, raiz->artista->nombre) > 0)
    {
        return BuscarArtista(raiz->derecha, artista);
    }
    else
    {
        return raiz->artista;
    }
    
}

NodoArb*InsertArtista(NodoArb *raiz, Artista *artista){
    if (raiz == NULL){
        crearNodoAVL(artista);
    }   
    int orden=ordenalf(artista->nombre, raiz->artista->nombre);

    if (orden < 0)
    {
        raiz->izquierda = InsertArtista(raiz->izquierda, artista);
    }
    else if (orden > 0)
    {
        raiz->derecha = InsertArtista(raiz->derecha, artista);
    }
    else
    {
        return raiz;
    }

    actualizarAltura(raiz);

    int balance = altura(raiz->izquierda) - altura(raiz->derecha);

    // rotación izquierda izquierda
    if (balance > 1 && strcmp(artista->nombre, raiz->izquierda->artista->nombre) < 0)
    {
        return RotarDer(raiz);
    }

    // rotación derecha derecha
    if (balance < -1 && strcmp(artista->nombre, raiz->derecha->artista->nombre) > 0)
    {
        return RotarIz(raiz);
    }

    // rtación izquierda derecha
    if (balance > 1 && strcmp(artista->nombre, raiz->izquierda->artista->nombre) > 0)
    {
        raiz->izquierda = RotarIz(raiz->izquierda);
        return RotarDer(raiz);
    }

    // rotación derecha izquierda
    if (balance < -1 && strcmp(artista->nombre, raiz->derecha->artista->nombre) < 0)
    {
        raiz->derecha = RotarDer(raiz->derecha);
        return RotarIz(raiz);
    }

    return raiz;
}



void BorrarArbol(NodoArb *raiz) {
    if (raiz==NULL)
        return;
    BorrarArbol(raiz->izquierda);

    BorrarArbol(raiz->derecha);
    
    Disco *disco = raiz->artista->discos;
    while (disco != NULL)
    {
        Disco *temp=disco;
        disco=disco->siguiente;
        BorrarDisco(temp);
    }

    free(raiz->artista);
    free(raiz);
        
}


NodoArb *cargarDatos(const char *directorio)
{
    char rutaArchivo[256];

    snprintf(rutaArchivo, sizeof(rutaArchivo), "%scatalogo_1000.csv", directorio); //para temas practicos usaremos el catalogo de 1000

    FILE *archivo = fopen(rutaArchivo, "r");
    if (archivo == NULL)
    {
        printf("Error al abrir catalogo_1000.csv\n");
        return NULL;
    }

    NodoArb *raiz = NULL;
    char Buffer[512];

    while (fgets(Buffer, sizeof(Buffer), archivo))
    {
        if (Buffer[0] == '#' || Buffer[0] == '\n'){

        }else{
            Buffer[strcspn(Buffer, "\n")] = '\0';

            char nombreArtista[100], nombreDisco[100], nombreCancion[100], Id[50];
            int popularidad, duracion;

            sscanf(Buffer, "%99[^;];%99[^;];%99[^;];%d;%d;%49s", 
                nombreArtista, nombreDisco, nombreCancion, &popularidad, &duracion, Id);

            Artista *artista = BuscarArtista(raiz, nombreArtista);
            if (artista == NULL)
            {
                artista = CrearArtista(nombreArtista);
                if (artista == NULL)
                    continue;
                raiz = InsertArtista(raiz,artista);
            }
            Disco *disc=BuscarDisco(artista,nombreDisco);
            Song *cancion= crearCancion(nombreCancion,Id,popularidad,duracion);
            insertCancion(disc,cancion);
        }
    }

    fclose(archivo);
    return raiz;
}

int contarCanciones(NodoArb *raiz){
    if(raiz==NULL) return 0;
    int suma=0;
    Disco *disc=raiz->artista->discos;
    while (disc!=NULL){
        Song *cancion=disc->canciones;
        while (cancion!=NULL){
            suma++;
            cancion=cancion->siguiente;
        }
        disc=disc->siguiente;
    }
    return suma+contarCanciones(raiz->izquierda)+contarCanciones(raiz->derecha);
}

Song *CancionId(NodoArb *raiz, char *id){
    if (raiz == NULL) return NULL;

    
    Disco *disco = raiz->artista->discos;
    while (disco != NULL) {
        Song *cancion = disco->canciones;
        while (cancion != NULL) {
            if (strcmp(cancion->Id, id) == 0)
                return cancion;
            cancion = cancion->siguiente;
        }
        disco = disco->siguiente;
    }

    
    Song *iz = CancionId(raiz->izquierda, id);
    if (iz != NULL) return iz;
    return CancionId(raiz->derecha, id);
}


Song *seleccionarCancion(Artista *artista, int numDisco, int numCancion){

    Disco *disco = artista->discos;
    int i = 1;
    while (disco != NULL && i < numDisco)
    {
        disco = disco->siguiente;
        i++;
    }

    if (disco == NULL)
    {
        printf("Disco no encontrado.\n");
        return NULL;
    }

    Song *cancion = disco->canciones;
    int j = 1;
    while (cancion != NULL && j < numCancion)
    {
        cancion = cancion->siguiente;
        j++;
    }

    if (cancion == NULL)
    {
        printf("Cancion no encontrada.\n");
        return NULL;
    }

    return cancion;
}

void ImprimirArtistas(NodoArb *raiz) {
    if (raiz == NULL) return;

    ImprimirArtistas(raiz->izquierda);
    if (raiz->artista != NULL) {
        printf("- %s\n", raiz->artista->nombre);
    }
    ImprimirArtistas(raiz->derecha);
}


void Catalogo(NodoArb *raiz){
        printf("\n=== Catalogo de Artistas Disponibles ===\n");
    if (raiz == NULL) {
        printf("No hay catalogo.\n");
    } else {
        ImprimirArtistas(raiz);
    }
    printf("----------------------------\n");
}
int ObtenerReproducciones(NodoArb *raiz, const char *track_id)
{
    if (raiz == NULL) return 0;

    Disco *disco = raiz->artista->discos;
    while (disco != NULL) {
        Song *cancion = disco->canciones;
        while (cancion != NULL) {
            if (strcmp(cancion->Id, track_id) == 0) {
                if (cancion->reproducciones > 0)
                return cancion->reproducciones;
            }
            cancion = cancion->siguiente;
        }
        disco = disco->siguiente;
    }

    int izq = ObtenerReproducciones(raiz->izquierda, track_id);
    if (izq > 0) return izq;
    return ObtenerReproducciones(raiz->derecha, track_id);
}

int ReproduccionesTotales(NodoArb *raiz) {
    if (raiz == NULL) return 0;
    int total = 0;
    Disco *disco = raiz->artista->discos;
    while (disco != NULL) {
        Song *cancion = disco->canciones;
        while (cancion != NULL) {
            if (cancion->reproducciones > 0)
        
            total += cancion->reproducciones;
            cancion = cancion->siguiente;
        }
        disco = disco->siguiente;
    }
    return total + ReproduccionesTotales(raiz->izquierda)+ ReproduccionesTotales(raiz->derecha);
}

void MaximaReproduccion(NodoArb *raiz, char *nom_out, char *art_out, int *max_out) {
    if (raiz == NULL) return;

    Disco *disco = raiz->artista->discos;
    while (disco != NULL) {
        Song *cancion = disco->canciones;
        while (cancion != NULL) {
            if (cancion->reproducciones > *max_out) {
                *max_out = cancion->reproducciones;
                strncpy(nom_out, cancion->cancion_name, 99);
                strncpy(art_out, raiz->artista->nombre, 99);
            }
            cancion = cancion->siguiente;
        }
        disco = disco->siguiente;
    }
    MaximaReproduccion(raiz->izquierda, nom_out, art_out, max_out);
    MaximaReproduccion(raiz->derecha,   nom_out, art_out, max_out);
}
