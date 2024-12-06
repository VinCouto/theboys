// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"
// seus #defines vão aqui
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS /5)
#define N_MISSOES (T_FIM_DO_MUNDO / 10)
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

struct bmp{
  int dist;
  int id_base;
};

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
 int base_ID;
 bool morto;
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
  //eventos iniciais
  ev_ini_herois(mundo_ini->herois,LEF);
  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}

//randomizador dos numeros que preencherão o vetor
int aleat (int min, int max){
  return (rand()%(max - min + 1) + 1);
}




void iniciarMundo(struct mundo *mundo_ini, struct mundo *mundo_fim){
    mundo_ini->Relogio = T_INICIO;
    mundo_fim->Relogio = T_FIM_DO_MUNDO;
    mundo_ini->Tam_Mundo.x = N_TAMANHO_MUNDO;
    mundo_ini->Tam_Mundo.y = N_TAMANHO_MUNDO;
    mundo_ini->N_habilidades = N_HABILIDADES;
    mundo_ini->N_herois = N_HEROIS;
    mundo_ini->N_bases = N_BASES;
    mundo_ini->N_missoes = N_MISSOES;
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
  for(int i = 0; i < N_HEROIS; i++){
    mundo_ini->herois[i].ID = i;
    mundo_ini->herois[i].experiencia = 0;
    mundo_ini->herois[i].paciencia = aleat(0,100);
    mundo_ini->herois[i].velocidade = aleat(50,5000);
    iniciarHabilidade(&mundo_ini->herois[i],mundo_ini->N_habilidades);
    mundo_ini->herois[i].morto = false;
  }
}

void iniciarBase(struct mundo *mundo_ini){
  mundo_ini->bases = (struct base*)malloc(mundo_ini->N_bases * sizeof(struct base));
  if(mundo_ini->bases == NULL)
    return;
  for(int i = 0; i < N_BASES; i ++){
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


void ev_ini_herois(struct s_heroi *heroi, struct fprio_t *LEF){
  for(int i = 0; i < N_HEROIS; i++){
    heroi[i].base_ID = aleat(0,N_BASES-1);
    int tempo = aleat(0,4320);
    struct evento *novo_evento;
    novo_evento = malloc(sizeof(struct evento*));
    novo_evento->base_ID = heroi[i].base_ID;
    novo_evento->heroi_ID = heroi[i].ID;
    novo_evento->prio = tempo;
    fprio_insere(LEF,novo_evento,CHEGA,tempo);
  }
}

void Chega(int instante, struct s_heroi *herois, struct base *base, struct fprio_t *LEF){
  if (herois->morto == true){
    return;
  }
  bool espera;
  herois->base_ID = base->ID;

  if((cjto_card(base->presentes) < base->lotação) && (base->espera == NULL)){
    espera = true;
  } else {
    espera = (herois->paciencia) > (10 * base->espera->tamanho);
  }

  struct evento *novo_evento;
  novo_evento = malloc(sizeof(struct evento*));
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
  if(herois->morto == true){
    return;
  }  
  lista_insere(base->espera, herois->ID, -1);
  struct evento *novo_evento;
  novo_evento = malloc(sizeof(struct evento*));
  novo_evento->base_ID = base->ID;
  novo_evento->heroi_ID = herois->ID;
  novo_evento->prio = instante;
  fprio_insere(LEF, novo_evento, AVISA, instante);
}

void Desiste(int instante, struct s_heroi *herois, struct base *base, struct fprio_t *LEF){
  if(herois->morto == true){
    return;
  }
  int destino = aleat(0,N_BASES);
  while(destino == base->ID){
    destino = aleat(0,N_BASES);
  }
  struct evento *novo_evento;
  novo_evento = malloc(sizeof(struct evento*));
  novo_evento->heroi_ID = herois->ID;
  novo_evento->base_ID = destino;
  novo_evento->prio = instante;
  fprio_insere(LEF,novo_evento, VIAJA, instante);
}

void Avisa(int instante, struct base *base, struct fprio_t *LEF){
  while(cjto_card(base->presentes) < base->lotação && base->espera->tamanho > 0){
      int id_heroi;
      lista_retira(base->espera, &id_heroi ,0);
      cjto_insere(base->presentes, id_heroi);
      struct evento *novo_evento;
      novo_evento = malloc(sizeof(struct evento*));
      novo_evento->base_ID = base->ID ;
      novo_evento->heroi_ID = id_heroi;
      novo_evento->prio = instante;
      fprio_insere(LEF,novo_evento,ENTRA,instante);
  }
}

void Entra(int instante, struct s_heroi *heroi, struct base *base, struct fprio_t *LEF){
  if(heroi->morto == true){
    return;
  }
  int PTB = 15 + (heroi->paciencia * aleat(1,20));
  struct evento *novo_evento;
  novo_evento = malloc(sizeof(struct evento*));
  novo_evento->base_ID = base->ID;
  novo_evento->heroi_ID = heroi->ID;
  novo_evento->prio = instante;
  fprio_insere(LEF,novo_evento,SAI,instante + PTB); 
}

void Sai(int instante, struct s_heroi *heroi, struct base *base, struct fprio_t *LEF){
  if(heroi->morto == true){
    return;
  }
  cjto_retira(base->presentes, heroi->ID);
  int destino = aleat(0,N_BASES);
  struct evento *novo_evento;
  novo_evento = malloc(sizeof(struct evento*));
  novo_evento->heroi_ID = heroi->ID;
  novo_evento->base_ID = destino;
  novo_evento->prio = instante;
  fprio_insere(LEF,novo_evento,VIAJA,instante);
  novo_evento->base_ID = base->ID;
  fprio_insere(LEF,novo_evento,AVISA,instante);
}

int DistanciaCart(struct coord atual, struct coord dest){
  int x = (dest.x - atual.x) * (dest.x - atual.x);
  int y = (dest.y - atual.y) * (dest.y - atual.y);
  int result = (int)sqrt(x + y);
  return result;
}

void Viaja(int instante, struct s_heroi *heroi, struct base *base, struct fprio_t *LEF){
  if(heroi->morto == true){
    return;
  }
  int atual = heroi->base_ID;
  int distancia = DistanciaCart(base[atual].local, base->local);
  int duração = distancia / heroi->velocidade;
  struct evento *novo_evento;
  novo_evento = malloc(sizeof(struct evento*));
  novo_evento->base_ID = base->ID;
  novo_evento->heroi_ID = heroi->base_ID;
  novo_evento->prio = instante + duração;
  fprio_insere(LEF,novo_evento,CHEGA,instante = duração);
}

void Morre(int instante, struct s_heroi *heroi, struct base *base, struct fprio_t *LEF){
  cjto_retira(base->presentes, heroi->base_ID);
  heroi->morto = true;
  struct evento *novo_evento;
  novo_evento = malloc(sizeof(struct evento*));
  novo_evento->base_ID = base->ID;
  novo_evento->prio = instante;
  fprio_insere(LEF,novo_evento, AVISA, instante);
}


int insertionSort(struct bmp vetor[]) {  
  for(int i = 1; i < N_BASES; i++){
    struct bmp atual = vetor[i];
    int j = i - 1;

    while(j >= 0 && vetor[j].dist > atual.dist){
      vetor[j + 1] = vetor[j];
      j--;
    }
    vetor[j+1] = atual;
  }
}

//funcao que recebe a struct missao e struct base e calcula qual a base
//mais proxima da missao e as ordena em um vetor
struct bmp* calcularBMP(struct missao *missao, struct base *base){
  struct bmp *bmp;
  bmp = malloc(N_BASES * sizeof(bmp));
  if(bmp == NULL)
    return;
  for(int i = 0; i < N_BASES; i++){
    int dist = DistanciaCart(missao->local,base[i].local);    
    bmp[i].dist = dist;
    bmp[i].id_base = base[i].ID;
  }
  bmp = insertionSort(bmp);
  return bmp;
}



void Missao(int instante, struct missao *missao, struct base *base, struct fprio_t *LEF){
  struct bmp *bmp;
  bmp = calcularBMP(missao,base);

}