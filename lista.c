#include <stdio.h>
#include <stdlib.h>
#include"lista.h"

struct lista_t *lista_cria (){
    struct lista_t *lst;
    lst = malloc(sizeof(struct lista_t));
    if (lst == NULL)
        return NULL;
    lst->prim = NULL;
    lst->ult = NULL;
    lst->tamanho = 0;
    return lst;

}


struct lista_t *lista_destroi (struct lista_t *lst){
    if (lst == NULL)
        return NULL;
    struct item_t *aux;

    while (lst->prim != NULL){
        aux = lst->prim;
        lst->prim = lst->prim->prox;
        free(aux);
        aux = NULL;
    }
    free(lst);
    return NULL;
}

int lista_insere (struct lista_t *lst, int item, int pos){
    if(lst == NULL)
        return -1;
    struct item_t *novo;
    if (!(novo = malloc(sizeof(struct item_t))))
        return -1;
    novo->valor = item;                 
    novo->ant = NULL;
    novo->prox = NULL;
    
    if (pos >= lst->tamanho || pos == -1){
        if (lst->tamanho == 0){       //verifica se é o primeiro item a ser colocado na lista
            lst->prim = novo;
            lst->ult = novo;
        }
        else {
            novo->ant = lst->ult;    //verifica se é o ultimo item a ser colocado na lista
            lst->ult->prox = novo;
            lst->ult = novo;
        }    
    }
    else if (pos == 0){
        novo->prox = lst->prim; //o primeiro vira segundo, ja q estamos colocando um item na primeira posicao
        lst->prim->ant = novo;
        lst->prim = novo;
        }
        else {
            struct item_t *aux = lst->prim;
            for (int i = 0; i < pos; i++)
                aux = aux->prox;
            novo->ant = aux->ant;
            novo->prox = aux;
            aux->ant->prox = novo;
            aux->ant = novo;
        }
    lst->tamanho++;
    return lst->tamanho;
}


int lista_retira (struct lista_t *lst, int *item, int pos){
    if(lst == NULL || lst->tamanho == 0)
        return -1;
    struct item_t *remover;
    if(pos >= lst->tamanho || pos == -1)    //o item a ser retirado eh o ultimo
        remover = lst->ult;
    else if(pos == 0)                     //o item a ser retirado eh o primeiro
        remover = lst->prim;
        else{
            remover = lst->prim;    //o item a se retirar esta no meio da lista
            for(int i = 0; i < pos; i++)
                remover = remover->prox;
        }
    if (remover->ant)       
        remover->ant->prox = remover->prox; //remove o anterior
    else                        
        lst->prim = remover->prox;      //ou entao remove o primeiro
    if (remover->prox)
        remover->prox->ant = remover->ant; //remove o proximo
    else
        lst->ult = remover->ant;        //ou remove o ultimo
    if (item != NULL)
        *item = remover->valor;         
    free(remover);                      //libera o item retirado
    lst->tamanho--;
    return lst->tamanho;
}


int lista_consulta (struct lista_t *lst, int *item, int pos){
    if(!(lst)|| !(item)|| lst->tamanho == 0 || pos >= lst->tamanho)  
        return -1;
    struct item_t *aux;
    if(pos == -1)
        aux = lst->ult;
    else{
        aux = lst->prim;       //consulta a lista ate achar a posicao
        for(int i = 0; i < pos; i++){
            aux = aux->prox;
        }
    }
    *item = aux->valor;        //armazena o valor da pos em um ponteiro do item
    return lst->tamanho;
}


int lista_procura (struct lista_t *lst, int valor){
    if(lst == NULL)
        return -1;
    struct item_t *aux;
    aux = lst->prim;
    int posicao = 0;
    while(aux){                     //avanca na lista ate achar o valor
        if (aux->valor == valor)
            return posicao;
        aux = aux->prox;
        posicao++;
    }   
    return -1;                     //retorna -1 se n encontrar
}

int lista_tamanho (struct lista_t *lst){
    if(lst == NULL)
        return -1;
    return lst->tamanho;
}

void lista_imprime (struct lista_t *lst){
    if(lst == NULL || lst->tamanho == 0)
        return;
    struct item_t *aux;
    aux = lst->prim;
    while(aux != NULL){
        printf("%d", aux->valor);
        if(aux->prox != NULL)   //verifica se ainda existem itens para imprimir
            printf(" ");        //coloca um " " caso ainda exista
        aux = aux->prox;
    }
}