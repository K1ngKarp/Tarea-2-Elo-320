#include "hash.h"

int tamanoIni(int cantidad){
    int p=ceil(log(cantidad/0.65)/log(2));
    return 2^(p+1); //tamaño sera una potencia de 2 cuyo factor de carga sea el menor posible.
}


TABLE_HASH *crear(int tamano) {
    TABLE_HASH *TH = malloc(sizeof(TABLE_HASH));
    
    if (TH==NULL)
        return NULL;

    TH->cantidadUsers=0;
    TH->capacidad=tamano;
    TH->tabla=malloc(sizeof(slot)*tamano);

    for (int i = 0; i < tamano; i++){
        
        TH->tabla[i].cabeza=NULL;
        TH->tabla[i].cont=0;
        
    }
    return TH;
}

unsigned int key(char *llave){
    unsigned int large=strlen(llave);
    unsigned long int valor=0;
    
    for (unsigned int i = 0; i < large; i++)
    {
        valor=valor*(31)+llave[i]*(i+1);
    }
    return valor;
}

unsigned int h(char *llave,int tamanoTH){
    int clave=key(llave);
    return clave%tamanoTH;

}

unsigned int SlotLibre(TABLE_HASH *TH ,int indx){
    int retorno; 
    for (int i = 1; i < TH->capacidad; i++)
    {
        retorno=(indx+(i*i))%TH->capacidad;

        if (TH->tabla[retorno].cont<2){
            
            return retorno;
        }
        
    }
    return TH->capacidad+1; //esto es si la tabla esta llena cosa que es imposible con las condiciones dadas
    
}


void insertarTH(TABLE_HASH *TH, char *nombre,char *pass, char *salt){
    if (TH == NULL || nombre == NULL || pass == NULL || salt == NULL)
    return;

    Usuario *user=malloc(sizeof(Usuario));
    
    if (user==NULL)
    return;
    
    strcpy(user->nombre,nombre);
    strcpy(user->pass,pass);
    strcpy(user->salt,salt);
    user->sgte=NULL;

    

    char *conct=concatenar(salt,pass);
    unsigned int llave=key(conct);

    unsigned int idx = h(llave,TH);

    free(conct);

    switch (TH->tabla[idx].cont){

    case 0:
        TH->tabla[idx].cabeza=user;
        TH->tabla[idx].cont=1;
        break;

    case 1:
        TH->tabla[idx].cabeza->sgte=user;
        TH->tabla[idx].cont++;
        break;

    default:

        int libre=SlotLibre(TH,idx);
        
        switch (TH->tabla[libre].cont){
            case 0:
                TH->tabla[libre].cabeza=user;
                TH->tabla[libre].cont=1;
                break;
            case 1:
                TH->tabla[libre].cabeza->sgte=user;
                TH->tabla[libre].cont++;
                break;

        break;
        }
    }

    TH->cantidadUsers++;
}


void LiberarTabla(TABLE_HASH *TH){

    if(TH==NULL) return;

    Usuario *indice;
    Usuario *ant;
    for (int i = 0; i < LARGE_KEY; i++){

        indice=TH->tabla[i].cabeza;
            
        while (indice!=NULL)
        {
            ant=indice;
            indice=indice->sgte;
            free(ant);
        }

        TH->tabla[i].cabeza=NULL;
        TH->tabla[i].cont=0;
        
    }
    TH->cantidadUsers=0;
    TH->capacidad=0;
    free(TH);
       
}


void imprimirTH(TABLE_HASH *TH){
    if (TH==NULL)
    {
        printf("Tabla vacia\n");
        return;
    }
    Usuario *rec;
    for (int i = 0; i < LARGE_KEY; i++)
    {
        if(TH->tabla[i].cabeza==NULL){
           // printf("slot %d vacio\n",(i+1));
           // printf("\n");

        }else{
            rec=TH->tabla[i].cabeza;
            printf("slot N°: %d \n",(i+1));
            

            while (rec!=NULL)
            {
                printf("nombre: %s \n",rec->nombre);
                printf("Pass: %s \n",rec->pass);
                printf("salt: %s \n",rec->salt);
                printf("\n");
                printf("slot N°: %d \n",(i+1));
                printf("cantidad de nodos: %d \n",TH->tabla[i].cont);
                rec=rec->sgte;
            }
            
        }
    }
    
}

/*
void imprimirColision(TABLE_HASH *TH, int LARGE_KEY){
    if (TH==NULL)
    {
        printf("Tabla vacia\n");
        return;
    }
    int contador=0;
    int mas3=0;
    int mas4=0;
    int mas5=0;
    for (int i = 0; i < LARGE_KEY; i++)
    {
        if (TH->tabla[i].cont >2)
        {
            printf("hay -> %d <- nodos en el slot -> %d <- \n", TH->tabla[i].cont,(i+1));
            printf("\n");
            contador+=TH->tabla[i].cont-2;
            switch (TH->tabla[i].cont)
            {
            case 3:
                mas3++;
                break;
            
            case 4:
                mas4++;
                break;
            }

            if (TH->tabla[i].cont >4)
            mas5++;
            
        }
        
    }
    printf("cantidad de colisiones %d \n",contador);
    printf("hay %d con 3 nodos\n", mas3);
    printf("hay %d con 4 nodos\n", mas4);
    printf("hay %d con 5 o mas nodos\n", mas5);
}
*/