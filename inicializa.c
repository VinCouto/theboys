#include "mundo.h"




void iniciarMundo(struct mundo *mundo_ini){
    mundo_ini->Relogio = T_INICIO;
    mundo_ini->Tam_Mundox = N_TAMANHO_MUNDO;
    mundo_ini->Tam_Mundoy = N_TAMANHO_MUNDO;
    mundo_ini->N_habilidades = N_HABILIDADES;
    mundo_ini->N_herois = N_HEROIS;
    mundo_ini->N_bases = N_BASES;
    mundo_ini->N_missoes = N_MISSOES;
    mundo_ini->ev_trat = 0;
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

struct s_heroi *iniciarHerois(struct mundo * mundo_ini){
  mundo_ini->herois = (struct s_heroi*)malloc(N_HEROIS * 
                                              sizeof(struct s_heroi));
  if(mundo_ini->herois == NULL){
    return mundo_ini->herois;
  }
  for(int i = 0; i < N_HEROIS; i++){
    mundo_ini->herois[i].ID = i;
    mundo_ini->herois[i].experiencia = 0;
    mundo_ini->herois[i].paciencia = aleat(0,100);
    mundo_ini->herois[i].velocidade = aleat(50,5000);
    iniciarHabilidade(&mundo_ini->herois[i],mundo_ini->N_habilidades);
    mundo_ini->herois[i].morto = false;
  }
  return mundo_ini->herois;
}

struct base *iniciarBase(struct mundo *mundo_ini){
  mundo_ini->bases = (struct base*)malloc(N_BASES * sizeof(struct base));
  if(mundo_ini->bases == NULL)
    return mundo_ini->bases;
  for(int i = 0; i < N_BASES; i ++){
    mundo_ini->bases[i].ID = i;
    mundo_ini->bases[i].localx = aleat(0,mundo_ini->Tam_Mundox-1);
    mundo_ini->bases[i].localy = aleat(0,mundo_ini->Tam_Mundoy-1);
    mundo_ini->bases[i].lotação = aleat(3,10);
    mundo_ini->bases[i].presentes = cjto_cria(mundo_ini->bases[i].lotação);
    mundo_ini->bases[i].espera = lista_cria();
    mundo_ini->bases[i].habilidades = cjto_cria(30);
    mundo_ini->bases[i].n_missao = 0;
  }
  return mundo_ini->bases;
}

struct missao *iniciarMissao(struct mundo *mundo_ini){
  mundo_ini->missoes = (struct missao*)malloc(N_MISSOES * sizeof(struct missao));
  if(mundo_ini->missoes == NULL)
    return mundo_ini->missoes;
  for(int i = 0; i < mundo_ini->N_missoes; i++){
    mundo_ini->missoes[i].ID = i;
    mundo_ini->missoes[i].localx = aleat(0,mundo_ini->Tam_Mundox-1);
    mundo_ini->missoes[i].localy = aleat(0,mundo_ini->Tam_Mundoy-1);
    iniciarHabilidadeMissao(&mundo_ini->missoes[i], mundo_ini->N_missoes);
    mundo_ini->missoes[i].N_perigo = aleat(0,100);
    mundo_ini->missoes->tent = 0;
  }
  return mundo_ini->missoes;
}
