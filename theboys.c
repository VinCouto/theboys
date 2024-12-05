// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"
// seus #defines vão aqui
#define CHEGA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRA 4
#define SAI 5
#define VIAJA 6
#define MORRE 7
#define MISSAO 8
#define FIM 9

// minimize o uso de variáveis globais


struct evento{
  int heroi_ID;
  int base_ID;
  int missao_ID;
  int prio;
};


struct coord{
  int x;
  int y;
};

struct s_heroi{
 int ID;
 struct cjto_t *habilidade;
 int paciencia;
 int velocidade;
 int experiencia;
 int base;
};

struct base{
  int ID;
  int lotação;
  struct cjto_t *presentes;
  struct lista_t *espera;
  struct coord local;
};

struct missao{
  int ID;
  struct cjto_t *habilidades;
  int N_perigo;
  struct coord local;
};

struct mundo{
  int N_herois;
  struct s_heroi *herois;
  int N_bases;
  struct base *bases;
  int N_missoes;
  struct missao*missoes;
  int N_habilidades;
  struct coord Tam_Mundo;
  int Relogio;
};

int aleat (int min, int max);

void iniciarMundo(struct mundo *mundo_ini, struct mundo *mundo_fim);

void iniciarHabilidade(struct s_heroi* heroi, int numHabilidades);

void iniciarHabilidadeMissao(struct missao* missao, int numHabilidades);

void iniciarHerois(struct mundo * mundo_ini);

void iniciarBase(struct mundo *mundo_ini);

void iniciarMissao(struct mundo *mundo_ini);

void Chega(int instante, struct s_heroi *herois, struct base *base, struct fprio_t *LEF);


// programa principal
int main (){
  // iniciar o mundo
  struct mundo *mundo_ini;
  struct mundo *mundo_fim;
  iniciarMundo(mundo_ini,mundo_fim);
  iniciarHerois(mundo_ini);
  iniciarBase(mundo_ini);
  iniciarMissao(mundo_ini);
  struct fprio_t *LEF = fprio_cria();
  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}

//randomizador dos numeros que preencherão o vetor
int aleat (int min, int max){
  return ((rand()%max + 1));
}




void iniciarMundo(struct mundo *mundo_ini, struct mundo *mundo_fim){
    mundo_ini->Relogio = 0;
    mundo_fim->Relogio = 525600;
    mundo_ini->Tam_Mundo.x = 20000;
    mundo_ini->Tam_Mundo.y = 20000;
    mundo_ini->N_habilidades = 10;
    mundo_ini->N_herois = mundo_ini->N_habilidades * 5;
    mundo_ini->N_bases = mundo_ini->N_herois / 5;
    mundo_ini->N_missoes = mundo_fim->Relogio / 100;
}

void iniciarHabilidade(struct s_heroi* heroi, int numHabilidades){
    heroi->habilidade = cjto_cria(numHabilidades);
    if(heroi->habilidade == NULL)
      return;
    int i = aleat(1,3);
    for(int j = 0; j < i; j++){
      int hab_aleat = aleat(0,numHabilidades);
      cjto_insere(heroi->habilidade,hab_aleat);
    }
}

void iniciarHabilidadeMissao(struct missao* missao, int numHabilidades){
    missao->habilidades = cjto_cria(numHabilidades);
    if(missao->habilidades == NULL)
      return;
    int i = aleat(1,3);
    for(int j = 0; j < i; j++){
      int hab_aleat = aleat(0,numHabilidades);
      cjto_insere(missao->habilidades,hab_aleat);
    }
}

void iniciarHerois(struct mundo * mundo_ini){
  mundo_ini->herois = (struct s_heroi*)malloc(mundo_ini->N_herois * 
                                              sizeof(struct s_heroi));
  if(mundo_ini->herois == NULL){
    return;
  }
  for(int i = 0; i < mundo_ini->N_herois; i++){
    mundo_ini->herois[i].ID = i;
    mundo_ini->herois[i].experiencia = 0;
    mundo_ini->herois[i].paciencia = aleat(0,100);
    mundo_ini->herois[i].velocidade = aleat(50,5000);
    iniciarHabilidade(&mundo_ini->herois[i],mundo_ini->N_habilidades);
  }
}

void iniciarBase(struct mundo *mundo_ini){
  mundo_ini->bases = (struct base*)malloc(mundo_ini->N_bases * sizeof(struct base));
  if(mundo_ini->bases == NULL)
    return;
  for(int i = 0; i , mundo_ini->N_bases; i ++){
    mundo_ini->bases[i].ID = i;
    mundo_ini->bases[i].local.x = aleat(0,mundo_ini->Tam_Mundo.x-1);
    mundo_ini->bases[i].local.x = aleat(0,mundo_ini->Tam_Mundo.y-1);
    mundo_ini->bases[i].lotação = aleat(3,10);
    mundo_ini->bases[i].presentes = cjto_cria(mundo_ini->bases[i].lotação);
    mundo_ini->bases[i].espera = lista_cria();
  }
}

void iniciarMissao(struct mundo *mundo_ini){
  mundo_ini->missoes = (struct missao*)malloc(mundo_ini->N_missoes * sizeof(struct missao));
  if(mundo_ini->missoes == NULL)
    return;
  for(int i = 0; i < mundo_ini->N_missoes; i++){
    mundo_ini->missoes[i].ID = i;
    mundo_ini->missoes[i].local.x = aleat(0,mundo_ini->Tam_Mundo.x-1);
    mundo_ini->missoes[i].local.y = aleat(0,mundo_ini->Tam_Mundo.y-1);
    iniciarHabilidadeMissao(&mundo_ini->missoes[i], mundo_ini->N_missoes);
    mundo_ini->missoes[i].N_perigo = aleat(0,100);
  }
}

void Chega(int instante, struct s_heroi *herois, struct base *base, struct fprio_t *LEF){
  bool espera;
  herois->base = base->ID;
  if((base->presentes->num < base->lotação) && (base->espera == NULL)){
    espera = true;
  } else {
    espera = (herois->paciencia) > (10 * base->espera->tamanho);
  }

  struct evento *novo_evento;
  novo_evento->base_ID = base->ID;
  novo_evento->heroi_ID = herois->ID;
  novo_evento->prio = instante;

  if(espera){
    fprio_insere(LEF, novo_evento, ESPERA, instante);
  } else{
    fprio_insere(LEF,novo_evento,DESISTE,instante);
  }
}

void Espera(int instante, struct s_heroi *herois, struct base *base, struct fprio_t *LEF){
  lista_insere(base->espera, herois, base->espera->ult);
  struct evento *novo_evento;
  novo_evento->base_ID = base->ID;
  novo_evento->heroi_ID = herois->ID;
  novo_evento->prio = instante;
  fprio_insere(LEF, novo_evento, AVISA, instante);
}