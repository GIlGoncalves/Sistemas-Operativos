#include "listaLigada.h"





/**
 * Inicia a lista ligada
 * @param l
 * @param c
 * @return 
 */

ListaLigada initLista() {
    ListaLigada l =NULL;
    l=(ListaLigada) malloc(sizeof(struct listaligada));
    l->nomeDiguest=NULL;
    return l;
}



ListaLigada initLista1(ListaLigada l,char *f) {
    if(l==NULL){
        l = (ListaLigada) malloc(sizeof(struct listaligada));
        l->nomeDiguest=strdup(f);
        l->prox = NULL;
    }
    return l;
}

/**
 * Insere elemento na lista ligada
 * @param l
 * @param c
 * @return 
 */
ListaLigada insereElemento(ListaLigada l, char *f) {
    if(l==NULL) {
        l = initLista1(l,f);
    } 
    else {			/*primeira insercao*/
    			if(l->nomeDiguest==NULL) {
    					
    					l->nomeDiguest=strdup(f);
    					l->prox=NULL;
    			}



			    		else {

			        ListaLigada novo=NULL;
			        novo = (ListaLigada) malloc(sizeof(struct listaligada));
			        novo->nomeDiguest=strdup(f);
			        novo->prox = l;
			        l = novo;
			       
   						 }
  }
    return l;
}


/**
 * Verifica se existe um ficheiro na lista ligada
 * @param l
 * @param c
 * @return 
 */


/**
 * Verifica se existe um determinado diguest da lista ligada
 * @param l
 * @param c
 * @return 
 */
int existeDiguest(ListaLigada l, char *c){
    int res = 0;
    ListaLigada aux = l;
    
    while(aux && !res){
        if(strcmp(aux->nomeDiguest,c) == 0) res = 1;
        aux = aux->prox;
    }


    return res;
}


/* Cria uma nova lista inserindo aqueles que são diferentes que 
* são para remover */
ListaLigada removeElemento(ListaLigada l, char *c){
    int removeu = 0;
    ListaLigada aux = l, novo = NULL;
    
    if(l==NULL) return NULL;




    /* Caso o elemento seja o 1o da lista */
    if(strcmp(aux->nomeDiguest, c) == 0){
        novo = aux;
        aux = aux->prox;
        free(novo);
        l = aux;
    }
    else {
        while(aux && !removeu){
            if(strcmp(aux->prox->nomeDiguest,c) == 0){
                novo = aux->prox;
                aux->prox = aux->prox->prox;
                free(novo);
                removeu = 1;
            }
            aux = aux->prox;
        }
    }

    return l;
}


int comprimentoListaLigada(ListaLigada l) {
    int c = 0;
    while(l != NULL){
        c++;
        l = l->prox;
    }
    return c;
}





int printLista(ListaLigada l) {

while(l) {

	printf("%s \n",l->nomeDiguest);
  l=l->prox;

}
 return 0;

}


int liberta(ListaLigada l){
ListaLigada aux =NULL;


while(l) {

        aux=l->prox;
        free(l);
        l=aux;
}

 return 0;

}



