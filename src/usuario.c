#include "usuario.h"
#include "grafo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

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

Usuario *login(TABLE_HASH *TH, char *nombre,char *pass){

    unsigned long index=key(nombre);

    for(int i=0;i<TH->capacidad;i++){
        unsigned long indice=h(index+i,TH->capacidad);
        Usuario *user=TH->tabla[indice];

        if (user == NULL) continue;

        if (strcmp(user->nombre, nombre) == 0) {
            char *concat = concatenar(user->salt, pass);
            if (concat == NULL) return NULL;

            if (strcmp(concat, user->pass) == 0) {
                free(concat);
                return user;
            }

            free(concat);
            return NULL;  
        }   
    }
    return NULL;
}

int Registrar(TABLE_HASH *TH, char *nombre, char *pass)
{
    if (TH == NULL || nombre == NULL || pass == NULL) return -1;

    // Verificar si ya existe
    if (login(TH, nombre, pass) != NULL) {
        printf("El usuario ya existe.\n");
        return -1;
    }

    char *salt = generarSalt();
    if (salt == NULL) return -1;

    char *pass_hash = concatenar(salt, pass);
    if (pass_hash == NULL) {
        free(salt);
        return -1;
    }

    insertarTH(TH, nombre, pass_hash, salt, "user");

    free(pass_hash);
    free(salt);
    return 0;
}


void MenuUser(Usuario *usuario, NodoArb *raiz, Grafo *grafo)
{
    int opcion;
    do
    {
        printf("\n=== Menu Principal ===\n");
        printf("1. Buscar artista\n");
        printf("2. Radio automatica\n");
        printf("3. Ver mi historial\n");
        printf("4. Lista de artistas\n");
        printf("5. Cerrar sesion\n");
        printf("> ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
        {
            char nombre[100];
            printf("Ingrese nombre de artista: ");
            scanf(" %99[^\n]", nombre);

            Artista *artista = BuscarArtista(raiz, nombre);
            if (artista == NULL)
            {
                printf("Artista no encontrado.\n");
                break;
            }

            mostrarDiscografia(artista);

            int numDisco, numCancion;
            printf("Ingrese numero de disco: ");
            scanf("%d", &numDisco);
            printf("Ingrese numero de cancion: ");
            scanf("%d", &numCancion);

            Song *cancion = seleccionarCancion(artista, numDisco, numCancion);
            if (cancion != NULL)
                reproducirCancion(usuario, artista, cancion);
            break;
        }

        case 2:
        {
            if (usuario->historialUser == NULL)
            {
                printf("No tienes canciones escuchadas aun.\n");
                break;
            }

            Song *ultima = usuario->historialUser->cancion;
            int idx_origen = -1;
            for (int i = 0; i < grafo->N; i++)
            {
                if (strcmp(grafo->atributos[i].track_id, ultima->Id) == 0)
                {
                    idx_origen = i;
                    break;
                }
            }

            if (idx_origen == -1)
            {
                printf("La cancion no esta en el grafo.\n");
                break;
            }

            Recomendacion resultado[3];
            int n = 0;
            bfs_recomendaciones(grafo, idx_origen, usuario, resultado, &n);

            if (n == 0)
            {
                printf("No hay recomendaciones disponibles.\n");
                break;
            }

            printf("\nRecomendaciones para %s:\n", usuario->nombre);
            for (int i = 0; i < n; i++)
            {
                int idx = resultado[i].indice;
                printf("%d. %s | %s [Similitud: %.2f]\n",
                       i + 1,
                       grafo->atributos[idx].nombre,
                       grafo->atributos[idx].artista,
                       resultado[i].peso);
            }

            char resp;
            printf("\nIngrese R para reproducir o S para saltar: ");
            scanf(" %c", &resp);

            if (resp == 'R' || resp == 'r')
            {
                for (int i = 0; i < n; i++)
                {
                    int idx = resultado[i].indice;
                    Artista *artista = BuscarArtista(raiz, grafo->atributos[idx].artista);
                    Song *cancion = CancionId(raiz, grafo->atributos[idx].track_id);
                    if (artista != NULL && cancion != NULL)
                        reproducirCancion(usuario, artista, cancion);
                }
            }
            break;
        }

        case 3:
        {
            printf("\n=== Historial de %s ===\n", usuario->nombre);
            Historial *h = usuario->historialUser;
            if (h == NULL)
            {
                printf("No tienes canciones escuchadas.\n");
                break;
            }
            int i = 1;
            while (h != NULL)
            {
                printf("%d. %s [%s]\n", i++, h->cancion->cancion_name, h->timestamp);
                h = h->siguiente;
            }
            break;
        }
        case 4:{
                
                Catalogo(raiz);
                break;
            }
        case 5:
            printf("Sesion cerrada. Hasta pronto, %s!\n", usuario->nombre);
            break;

        default:
            printf("Opcion invalida.\n");
            break;
        }
    } while (opcion != 5);
}

void MenuAdmin(Usuario *admin, TABLE_HASH *TH, NodoArb *raiz, Grafo *grafo)
{
    (void)admin;
    int opcion;

    do
    {
        printf("\n-- Menu Administrador --\n");
        printf("1. Estadisticas globales\n");
        printf("2. Reporte Heavy Hitters\n");
        printf("3. Lista de usuarios\n");
        printf("4. Cerrar Programa\n");
        printf("-> ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
        {
            printf("\n=== Estadisticas Globales ===\n");
            printf("Canciones en catalogo : %d\n", contarCanciones(raiz));
            printf("Aristas en grafo      : %d\n", contar_aristas(grafo));
            printf("Usuarios registrados  : %d\n", TH->capacidad);

            int total = ReproduccionesTotales(raiz);
            printf("Reproducciones totales: %d\n", total);

            char nom_c[100] = "", art_c[100] = "";
            int max_c = 0;
            MaximaReproduccion(raiz, nom_c, art_c, &max_c);
            if (max_c > 0)
                printf("Cancion mas reproducida: %s | %s (%d repr.)\n",
                       nom_c, art_c, max_c);

            char art_top[100] = "";
            int max_a = 0;
            artista_mas_escuchado(TH, grafo, art_top, &max_a);
            if (max_a > 0)
                printf("Artista mas escuchado : %s (%d repr.)\n",
                       art_top, max_a);
            break;
        }

        case 2:
            heavy_hitters(grafo, raiz);
            break;

        case 3:
            printf("\n=== Listado de Usuarios ===\n");
            for (int i = 0; i < TH->capacidad; i++)
            {
                if (TH->tabla[i] != NULL)
                {
                    int escuchadas = 0;
                    Historial *h = TH->tabla[i]->historialUser;
                    while (h != NULL) { escuchadas++; h = h->siguiente; }
                    printf("- %s    ||   Total escuchadas: %d\n",
                           TH->tabla[i]->nombre,escuchadas);
                }
            }
            break;

        case 4:
            printf("Saliendo del modo administrador...\n");
            break;

        default:
            printf("Opcion invalida.\n");
            break;
        }
    } while (opcion != 4);
}