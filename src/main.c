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
    if (argc != 4)
    {
        printf("Uso: ./vibenodes <-lista|-matriz> <-user|-admin> <directorio>\n");
        return 1;
    }

    int matriz = 0;
    if (strcmp(argv[1], "-matriz") == 0)
        matriz = 1;
    else if (strcmp(argv[1], "-lista") == 0)
        matriz = 0;
    else
    {
        printf("Modo grafo invalido. Use -lista o -matriz.\n");
        return 1;
    }

    int admin = 0;
    if (strcmp(argv[2], "-admin") == 0)
        admin = 1;
    else if (strcmp(argv[2], "-user") == 0)
        admin = 0;
    else
    {
        printf("Modo usuario invalido. Use -user o -admin.\n");
        return 1;
    }

    char *directorio = argv[3];
    srand(time(NULL));

    printf("=== VibeNodes ===\n");
    printf("Cargando datos desde '%s'...\n", directorio);
    clock_t t1, t2;


    t1 = clock();

    NodoArb *raiz = cargarDatos(directorio);
    t2 = clock();
    printf("Datos cargados en %.2f segundos.\n", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);
    int N = contarCanciones(raiz);

    t1 = clock();
    Grafo *grafo = cargar_grafo(directorio, N, matriz, raiz);
    t2 = clock();
    printf("Grafo cargado en %.2f segundos.\n", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);

    t1 = clock();
    
    TABLE_HASH *TH = CargarTabla(directorio);
    t2 = clock();
    printf("Usuarios cargados en %.2f segundos.\n", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);

    if (raiz == NULL || grafo == NULL || TH == NULL)
    {
        printf("Error al cargar datos.\n");
        if (raiz)
            BorrarArbol(raiz);
        if (grafo)
            liberar_grafo(grafo);
        if (TH)
            LiberarTabla(TH);
        return 1;
    }
    t1 = clock();

    CargarReproductor(TH, raiz, directorio);
    t2 = clock();
    printf("Reproducciones cargadas en %.2f segundos.\n", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);

    if (admin)
    {
        printf("\n=== VibeNodes [Modo Administrador] ===\n");

        char username[50], password[100];
        Usuario *admin = NULL;

        while (admin == NULL)
        {
            printf("Ingrese usuario administrador: ");
            scanf(" %49[^\n]", username);
            printf("Ingrese contrasena: ");
            scanf(" %99[^\n]", password);
            
            
            Usuario *log = login(TH, username, password);

            if (log == NULL)
                printf("Usuario o contrasena incorrectos.\n");
            else if (strcmp(log->tipo, "admin") != 0)
                printf("Este usuario no es administrador.\n");
            else
                admin = log;

            memset(password, 0, sizeof(password));
        }

        printf("Autenticacion exitosa. Bienvenido, %s!\n", admin->nombre);
        MenuAdmin(admin, TH, raiz, grafo);
    }
    else
    {
        printf("\n=== VibeNodes ===\n");

        char username[50], password[100];
        Usuario *usuario = NULL;
        int opcion;

        while (usuario == NULL)
        {
            printf("1. Iniciar sesion\n");
            printf("2. Registrarse\n");
            printf("> ");
            scanf("%d", &opcion);

            switch (opcion)
            {
            case 1:
                printf("Ingrese nombre de usuario: ");
                scanf(" %49[^\n]", username);
                printf("Ingrese contrasena: ");
                scanf(" %99[^\n]", password);

                Usuario *encontrado = login(TH, username, password);
                if (encontrado == NULL)
                    printf("Usuario o contrasena incorrectos.\n");
                else if (strcmp(encontrado->tipo, "admin") == 0)
                    printf("Use -admin para acceder como administrador.\n");
                else
                    usuario = encontrado;

                memset(password, 0, sizeof(password));
                break;

            case 2:
                printf("Ingrese nombre de usuario: ");
                scanf(" %49[^\n]", username);
                printf("Ingrese contrasena: ");
                scanf(" %99[^\n]", password);

                if (Registrar(TH, username, password) == 0)
                    usuario = login(TH, username, password);

                memset(password, 0, sizeof(password));
                break;

            default:
                printf("Opcion invalida.\n");
                break;
            }
        }

        printf("Bienvenido, %s!\n", usuario->nombre);
        MenuUser(usuario, raiz, grafo);
    }

    GuardarReproductor(TH,directorio);
    GuardarTabla(TH,directorio);

    LiberarTabla(TH);
    BorrarArbol(raiz);
    liberar_grafo(grafo);
    printf("Liberando memoria... \n");

    return 0;
}