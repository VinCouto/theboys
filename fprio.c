#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t *fprio_cria (){
    struct fprio_t *f;
    f = malloc(sizeof(struct fprio_t));
    if (f == NULL)
        return NULL;
    f->prim = NULL;
    f->num = 0;
    return f;
}
struct fprio_t *fprio_destroi (struct fprio_t *f){
    if(f == NULL){
        free(f);
        return NULL;
    }
    struct fpnodo_t *aux;
    while (f->prim != NULL){
        free(f->prim->item);
        aux = f->prim;
        f->prim = f->prim->prox;
        free (aux);
    }
    free(f);
    return NULL;
}



int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio){
    if(!f || !item)
        return -1;
    struct fpnodo_t *aux = f->prim;
    while(aux){         //verifica se eh um item repitido
        if(aux->item == item)
            return -1;
        aux = aux->prox;
    }
    struct fpnodo_t *novo = malloc(sizeof(struct fpnodo_t));
    if(!novo)
        return -1;
    novo->item = item;
    novo->prio = prio;
    novo->tipo = tipo;
    novo->prox = NULL;
    //verifica se a fila ainda nao começou ou se é uma nova prioridade
    if(!f->prim || f->prim->prio > prio){      
        novo->prox = f->prim;
        f->prim = novo;
    }else {   //avança na fila ate chegar no final da fila ou da sua prioridade
        aux = f->prim;
        while(aux->prox && aux->prox->prio <= prio)
            aux = aux->prox;
        novo->prox = aux->prox;
        aux->prox = novo;
        }
    f->num++;
    return f->num;
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio){
    if(!f || !f->prim)
        return NULL;
    struct fpnodo_t *remover = f->prim;
    void *item = remover->item;
    if(item == NULL)    //verifica se algum dos parametros é nulo
        return NULL;
    if(tipo == NULL)
        return NULL;
    *tipo = remover->tipo;
    
    if(prio == NULL)
        return NULL;
    *prio = remover->prio;
    f->prim = remover->prox;
    free(remover);
    f->num--;
    return item;
}


int fprio_tamanho (struct fprio_t *f){
    if(!f)
        return -1;
    return f->num;
}


void fprio_imprime (struct fprio_t *f){
    if(!f || !f->prim)
        return;
    struct fpnodo_t *aux = f->prim;
    while(aux){
        printf("(%d %d)", aux->tipo, aux->prio);
        if(aux->prox)
            printf(" ");
        aux = aux->prox;
    }
}