#include "mundo.h"

//randomizador dos numeros que preencherão o vetor
int aleat (int min, int max){
  return (rand()%(max - min) + min);
}


void agendarevento(struct fprio_t *LEF, int tipo, int T, int H, int B, int M){
  struct evento * novo_evento;
  novo_evento = malloc(sizeof(struct evento));
  novo_evento->heroi_ID = H;
  novo_evento->base_ID = B;
  novo_evento->missao_ID = M;
  novo_evento->prio = T;
  fprio_insere(LEF,novo_evento,tipo,T);
}

void ev_ini_herois(struct s_heroi *heroi, struct fprio_t *LEF){
  for(int i = 0; i < N_HEROIS; i++){
    heroi[i].base_ID = aleat(0,N_BASES-1);
    int tempo = aleat(0,4320);
    agendarevento(LEF,CHEGA,tempo,heroi[i].ID,heroi[i].base_ID, -1);
  }
}

void ev_ini_missoes(struct missao *missao, struct fprio_t *LEF){
  for(int i = 0; i < N_MISSOES; i++){
    int tempo = aleat(0,T_FIM_DO_MUNDO);
    agendarevento(LEF,MISSAO,tempo,-1,-1,missao[i].ID);
  }
}

void ev_fim_do_mundo(struct fprio_t *LEF){
  agendarevento(LEF,FIM,T_FIM_DO_MUNDO,-1,-1,-1);
}

void Chega(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF){
  if (M->herois[H].morto == true){
    return;
  }
  M->ev_trat++;
  int espera = 0;
  M->herois[H].base_ID = M->bases[B].ID;

  if((cjto_card(M->bases[B].presentes) < M->bases[B].lotação) && (M->bases[B].espera->tamanho == 0)){
    espera = true;
  } else {
    if((M->herois[H].paciencia) > (10 * M->bases[B].espera->tamanho))
      espera = 1;
  }


  if(espera != 0){
    printf("%6d: CHEGA  HEROI %2d BASE %d ", instante, M->herois[H].ID, M->bases[B].ID);
    printf("(%2d/%2d) ESPERA \n", cjto_card(M->bases[B].presentes), M->bases[B].lotação);
    agendarevento(LEF, ESPERA, instante, M->herois[H].ID, M->bases[B].ID, 0);
  } else{
    printf("%6d: CHEGA  HEROI %2d BASE %d ", instante, M->herois[H].ID, M->bases[B].ID);
    printf("(%2d/%2d) DESISTE \n", cjto_card(M->bases[B].presentes), M->bases[B].lotação);
    agendarevento(LEF, DESISTE, instante, M->herois[H].ID, M->bases[B].ID, -1);
  }
}

void Espera(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF){
  if(M->herois[H].morto == true){
    return;
  }  
  M->ev_trat++;
  lista_insere(M->bases[B].espera, M->herois[H].ID, -1);
  if(M->bases[B].espera->tamanho > M->bases[B].tam_max)
    M->bases[B].tam_max = M->bases[B].espera->tamanho;
  printf("%6d: ESPERA HEROI %2d ", instante,M->herois[H].ID);
  printf("BASE %d (%2d) \n", M->bases[B].ID,M->bases[B].espera->tamanho);
  agendarevento(LEF, AVISA, instante, M->herois[H].ID, M->bases[B].ID, -1);
}

void Desiste(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF){
  if(M->herois[H].morto == true){
    return;
  }
  M->ev_trat++;
  int destino = aleat(0,N_BASES-1);
  while(destino == B){
    destino = aleat(0,N_BASES);
  }
  printf("%6d: DESIST HEROI %2d ", instante, M->herois[H].base_ID);
  printf("BASE %2d \n", M->bases[B].ID);
  agendarevento(LEF, VIAJA, instante, M->herois[H].ID, destino, -1);
}

void Avisa(int instante, struct mundo *M,  int B, struct fprio_t *LEF){
  printf("%6d: AVISA  PORTEIRO BASE %d ", instante, M->bases[B].ID);
  printf("(%2d/%2d) FILA", cjto_card(M->bases[B].presentes), M->bases[B].lotação);
  printf("[");
  lista_imprime(M->bases[B].espera);
  printf("]\n");
  while(cjto_card(M->bases[B].presentes) < M->bases[B].lotação && M->bases[B].espera->tamanho > 0){
    int id_heroi;
    lista_retira(M->bases[B].espera, &id_heroi ,0);
    cjto_insere(M->bases[B].presentes, id_heroi);
    printf("%6d: AVISA  PORTEIRO BASE %d ", instante, M->bases[B].ID);
    printf("ADMITE %2d \n", id_heroi);
    agendarevento(LEF, ENTRA, instante, id_heroi, M->bases[B].ID, -1);
  }
}

void Entra(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF){
  if(M->herois[H].morto == true){
    return;
  }
  M->ev_trat++;
  int PTB = 15 + (M->herois[H].paciencia * aleat(1,20));
  cjto_insere(M->bases[B].presentes,M->herois[H].ID);
  struct cjto_t *temp;
  temp = cjto_uniao(M->bases[B].habilidades,M->herois[H].habilidade);
  cjto_destroi(M->bases[B].habilidades);
  M->bases[B].habilidades = temp;
  printf("%6d: ENTRA  HEROI %2d BASE ", instante,  M->herois[H].ID);
  printf("%d (%2d/", M->bases[B].ID, cjto_card(M->bases[B].presentes));
  printf("%2d) SAI %d\n", M->bases[B].lotação, (instante + PTB));
  agendarevento(LEF, SAI, instante + PTB, M->herois[H].ID, M->bases[B].ID, -1);
}

void Sai(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF){
  if(M->herois[H].morto == true){
    return;
  }
  M->ev_trat++;
  int destino = aleat(0,N_BASES-1);
  struct cjto_t *temp;
  temp = cjto_dif(M->bases[B].habilidades, M->herois[H].habilidade);
  cjto_destroi(M->bases[B].habilidades);
  M->bases[B].habilidades = temp;
  printf("%6d: SAI    HEROI %2d BASE ", instante, M->herois[H].ID);
  printf("%d (%2d/", M->bases[B].ID, cjto_card(M->bases[B].presentes));
  printf("%2d)\n", M->bases[B].lotação);
  agendarevento(LEF, VIAJA, instante, M->herois[H].ID, destino, -1);
  agendarevento(LEF, AVISA, instante, M->herois[H].ID, M->bases[B].ID, -1);
  cjto_retira(M->bases[B].presentes, M->herois[H].ID);
}

int DistanciaCart(int atualx, int atualy, int destx, int desty){
  int x = (destx - atualx) * (destx - atualx);
  int y = (desty - atualy) * (desty - atualy);
  int result = (int)sqrt(x + y);
  return result;
}

void Viaja(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF){
  if(M->herois[H].morto == true){
    return;
  }
  M->ev_trat++;
  int atual = M->herois[H].base_ID;
  int distancia = DistanciaCart(M->bases[atual].localx, M->bases[atual].localy, M->bases[B].localx, M->bases[B].localy);
  int duração = distancia / M->herois[H].velocidade;
  printf("%6d: VIAJA  HEROI %2d BASE ", instante, M->herois[H].ID);
  printf("%d BASE %d DIST %d ", M->bases[atual].ID, B, distancia);
  printf("VEL %d CHEGA %d\n", M->herois[H].velocidade, (instante + duração));
  agendarevento(LEF, CHEGA, (instante + duração), M->herois[H].ID, M->bases[B].ID, -1);
}

void Morre(int instante, struct mundo *M, int H, int B, int missao,
                                                      struct fprio_t *LEF){
  M->ev_trat++;
  cjto_retira(M->bases[B].presentes, M->herois[H].base_ID);
  M->herois[H].morto = true;
  M->num_mortos++;
  printf("%6d: MORRE  HEROI %2d ", instante, M->herois[H].ID);
  printf("MISSAO %d", missao);
  agendarevento(LEF, AVISA, instante, -1, M->bases[B].ID, -1);
}


void Missao(int instante, struct mundo *M, int id_missao, struct fprio_t *LEF){
  M->ev_trat++;
  int BMP = -1;
  int menordist = N_TAMANHO_MUNDO * N_TAMANHO_MUNDO;
  for(int i = 0; i < N_BASES; i++){
    int dist = DistanciaCart(M->missoes[id_missao].localx,
    M->missoes[id_missao].localy,M->bases[i].localx, M->bases[i].localy);
    printf("%6d: MISSAO %d ", instante, M->missoes[id_missao].ID);
    printf("BASE %d DIST %d HEROIS [", M->bases[i].ID, dist);
    cjto_imprime(M->bases[i].presentes);
    printf("]\n");
    printf("%6d: MISSAO %d ", instante, M->missoes[id_missao].ID);
    for(int j = 0; j < N_HEROIS; j++){
      if(cjto_pertence(M->bases[i].presentes, M->herois[j].ID)){
        printf("%6d: MISSAO %d ", instante, M->missoes[id_missao].ID);
        printf("HAB HEROI %2d: [", M->herois[j].ID);
        cjto_imprime(M->herois[j].habilidade);
        printf("]\n");
      }
    }
    int contem = cjto_contem(M->bases[i].habilidades,M->missoes[i].habilidades);
    if(contem){

      if(menordist > dist)
        BMP = M->bases[i].ID;
    }
  }
  //missao cumprida//
  if(BMP != -1){
    M->bases[BMP].n_missao++;
    M->miss_cumpr++;
    if(M->max_miss < M->missoes[id_missao].tent)
      M->max_miss = M->missoes[id_missao].tent;
    if(M->min_miss > M->missoes[id_missao].tent)
      M->min_miss = M->missoes[id_missao].tent;
    
    printf("%6d: MISSAO %d ", instante, M->missoes[id_missao].ID);
    printf("TENT %d HAB REQ: [", M->missoes[id_missao].tent);
    cjto_imprime(M->missoes[id_missao].habilidades);
    printf("]\n");

    printf("%6d: MISSAO %d CUMPRIDA ", instante, M->missoes[id_missao].ID);
    printf("BASE %d HABS: [", M->bases[BMP].ID);
    cjto_imprime(M->missoes[id_missao].habilidades);
    printf("]\n");
    for(int i = 0; i < N_HEROIS; i++){
      if(cjto_pertence(M->bases[BMP].presentes,M->herois[i].ID)){
        int risco = M->missoes[id_missao].N_perigo / (M->herois[i]. paciencia + M->herois[i].experiencia + 1);
        if (risco > aleat(0,30))
          agendarevento(LEF, MORRE, instante, M->herois[i].ID, M->bases[BMP].ID, -1);
        else
          M->herois[i].experiencia++;
      }
    
    }
  } else {
    printf("%6d: MISSAO %d IMPOSSIVEL\n", instante, M->missoes[id_missao].ID);
    M->missoes[id_missao].tent++;
    agendarevento(LEF, MISSAO, instante + 24*60, -1, -1 , id_missao);
  }

}

void Fim(int tempo, struct mundo *M){
  M->ev_trat++;
  for(int i = 0; i < N_HEROIS; i++){
    if(M->herois[i].morto == false){
      printf("HEROI %2d VIVO  PAC %3d",M->herois[i].ID,M->herois[i].paciencia);
      printf(" VEL %4d EXP ", M->herois[i].velocidade);
      printf("%4d HABS [", M->herois[i].experiencia);
      cjto_imprime(M->herois[i].habilidade);
      printf("]\n");
    } else{
      printf("HEROI %2d MORTO ",M->herois[i].ID);
      printf("PAC %3d ",M->herois[i].paciencia);
      printf("VEL %4d EXP ", M->herois[i].velocidade);
      printf("%4d HABS [", M->herois[i].experiencia);
      cjto_imprime(M->herois[i].habilidade);
      printf("]\n");
    }
  }

  for(int i = 0; i < N_BASES; i++){
    printf("BASE %2d LOT %2d ", M->bases[i].ID, M->bases[i].lotação); 
    printf("FILA MAX %2d ", M->bases[i].tam_max);  //colocar tam max fila
    printf("MISSOES %d\n", M->bases[i].n_missao);  
  }
  printf("EVENTOS TRATADOS: %d\n", M->ev_trat);
  printf("MISSSOES CUMPRIDAS: %d/%d ", M->miss_cumpr,N_MISSOES);
  float media_miss = M->miss_cumpr/N_MISSOES;
  printf("(%1.f%%\n)",media_miss);
  printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, ", M->min_miss, M->max_miss);
  float media_tent = M->min_miss/ M->max_miss;
  printf("MEDIA %.1f", media_tent);
  float txa_morte = M->num_mortos / N_HEROIS;
  printf("TAXA DE MORTALIDADE : %.1f%%", txa_morte);
  printf("TEMPO %d\n", tempo);


    for(int i = 0; i < N_BASES; i++){
    cjto_destroi(M->bases[i].presentes);
    cjto_destroi(M->bases[i].habilidades);
    lista_destroi(M->bases[i].espera);
  }
  for(int i = 0; i < N_HEROIS; i++){
    cjto_destroi(M->herois[i].habilidade);
  }
  for(int i = 0; i < N_MISSOES; i++){
    cjto_destroi(M->missoes[i].habilidades);
  }
  free(M->bases);
  free(M->herois);
  free(M->missoes);
  free(M);
}
