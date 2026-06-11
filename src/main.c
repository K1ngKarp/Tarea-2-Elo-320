#include "hash.h"
#include "usuario.h"
#include "avl.h"
#include "utils.h"

int main(int argc, char *argv[]){
    if (argc!=4){
        printf("Formato incorrecto.\n");
        printf("Formato ideal:\n");
        printf("./vibenodes -lista -user data/\n") ;
        printf("./vibenodes -matriz -admin data/ \n");
        return 1;
    }
    
    char *estructura=argv[1];
    char *modo      =argv[2];
    char *data      =argv[3];

    return 0;
}