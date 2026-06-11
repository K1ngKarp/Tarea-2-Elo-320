#include "avl.c"

int main(){

    char *f="data/catalogo_100.csv";
    
    Artistas *raiz=llenarArb(f);

    iprimirAVL(raiz);
    BorrarAVL(raiz);
    
    return 0;
}