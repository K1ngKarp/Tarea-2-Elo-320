#include "hash.h"
#include "usuario.h"
#include "avl.h"
#include "utils.h"
#include "grafo.h"
#include "reproductor.h"
#include "usuario.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>



int main(int argc, char *argv[]){
    if (argc != 4) {
        printf("Uso: ./vibenodes <-lista|-matriz> <-user|-admin> <directorio>\n");
        return 1;
    }
    int usar_matriz = 0;
    if (strcmp(argv[1], "-matriz") == 0) usar_matriz = 1;
    else if (strcmp(argv[1], "-lista") == 0) usar_matriz = 0;
    else {
        printf("Modo grafo inválido. Use -lista o -matriz.\n");
        return 1;
    }

    int modo_admin = 0;
    if (strcmp(argv[2], "-admin") == 0) modo_admin = 1;
    else if (strcmp(argv[2], "-user") == 0) modo_admin = 0;
    else {
        printf("Modo usuario inválido. Use -user o -admin.\n");
        return 1;
    }

    char *directorio = argv[3];
    srand(time(NULL));

    printf("=== VibeNodes ===\n");
    printf("Cargando datos desde '%s'...\n", directorio);

    clock_t t1, t2;

    // 1. Cargar Catálogo (AVL)
    t1 = clock();
    NodoArb *raiz = cargarDatos(directorio);
    t2 = clock();
    printf("- Catálogo cargado en %.2f ms\n", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);

    if (raiz == NULL) {
        printf("Error al cargar el catálogo.\n");
        return 1;
    }

    int N = contarCanciones(raiz);

    // 2. Cargar Grafo
    t1 = clock();
    Grafo *grafo = cargar_grafo(directorio, N, usar_matriz, raiz);
    t2 = clock();
    printf("- Grafo cargado en %.2f ms (%s)\n", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000, usar_matriz ? "Matriz" : "Lista");

    // 3. Cargar Usuarios
    t1 = clock();
    TABLE_HASH *TH = cargarTH(directorio);
    t2 = clock();
    printf("- Usuarios cargados en %.2f ms\n", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);

    if (TH == NULL) {
        printf("Error al cargar usuarios.\n");
        BorrarArbol(raiz);
        liberar_grafo(grafo);
        return 1;
    }

    // 4. Cargar Reproducciones
    t1 = clock();
    CargarReproductor(TH, raiz, directorio);
    t2 = clock();
    printf("- Reproducciones cargadas en %.2f ms\n", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);

    //MENÚ 
    if (modo_admin) {
        printf("\n=== VibeNodes [Modo Administrador] ===\n");

        char username[100] = {0}, password[100] = {0};
        Usuario *admin_user = NULL;

        while (admin_user == NULL) {
            printf("Ingrese usuario administrador: ");
            scanf(" %99[^\n]", username);
            printf("Ingrese contraseña: ");
            scanf(" %99[^\n]", password);

            Usuario *log = login(TH, username, password);

            if (log == NULL) {
                printf("Usuario o contraseña incorrectos.\n");
            } else if (strcmp(log->tipo, "admin") != 0) {
                printf("Este usuario no tiene privilegios de administrador.\n");
            } else {
                admin_user = log;
            }

            memset(password, 0, sizeof(password));
        }

        printf("Autenticación exitosa. Bienvenido, %s!\n", admin_user->nombre);
        MenuAdmin(admin_user, TH, raiz, grafo);

    } else {
        // Modo Usuario normal
        printf("\n=== VibeNodes ===\n");

        char username[50];
        char password[100];
        Usuario *usuario = NULL;
        int opcion;

        while (usuario == NULL) {
            printf("1. Iniciar sesión\n");
            printf("2. Registrarse\n> ");
            scanf("%d", &opcion); 

            if (opcion == 1) {
                printf("Ingrese nombre de usuario: ");
                scanf(" %99[^\n]", username);
                printf("Ingrese contraseña: ");
                scanf(" %99[^\n]", password);

                Usuario *encontrado = login(TH, username, password);
                printf("asdfa\n");
                if (encontrado == NULL) {
                    printf("Usuario o contraseña incorrectos.\n");
                } else {
                    printf("asdfa\n");
                    usuario = encontrado;
                }
                memset(password, 0, sizeof(password));
                printf("dsfsdf\n");
                break;
            } 
            else if (opcion == 2) {
                printf("Ingrese nombre de usuario: ");
                scanf(" %99[^\n]", username);
                printf("Ingrese contraseña: ");
                scanf(" %99[^\n]", password);

                if (Registrar(TH, username, password) == 0) {
                    usuario = login(TH, username, password);
                }
                memset(password, 0, sizeof(password));
                break;

            } else {
                printf("Opción inválida.\n");
                break;
            }

            
        }

        printf("¡Bienvenido, %s!\n", usuario->nombre);
        MenuUser(usuario, raiz, grafo);
    }

    printf("\nGuardando datos...\n");
    GuardarReproductor(TH, directorio);
    GuardarTabla(TH, directorio);  
    printf("Liberando memoria...\n");
    LiberarTabla(TH);
    BorrarArbol(raiz);
    liberar_grafo(grafo);

    printf("¡Hasta pronto!\n");
    return 0;
}