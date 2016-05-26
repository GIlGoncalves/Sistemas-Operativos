#ifndef LLIGADA_H
#define LLIGADA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Vai servir para guarda os ficheiros e o respectivo diguest
 */

typedef struct listaligada {
    char *nomeDiguest;
    struct listaligada *prox;
} *ListaLigada;

ListaLigada initLista();

ListaLigada initLista1(ListaLigada l, char *c);

ListaLigada insereElemento(ListaLigada l, char *c);



int existeDiguest(ListaLigada l, char *c);

ListaLigada removeElemento(ListaLigada l, char *c);

int comprimentoListaLigada(ListaLigada l);
int printLista(ListaLigada l);
int liberta(ListaLigada l);





#endif
