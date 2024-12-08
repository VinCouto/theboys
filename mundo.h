#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS /5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)

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




struct evento{
  int heroi_ID;
  int base_ID;
  int missao_ID;
  int prio;
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
  int localx;
  int localy;
  int n_missao;
  int tam_max;
  struct cjto_t *habilidades;
};

struct missao{
  int ID;
  struct cjto_t *habilidades;
  int N_perigo;
  int localx;
  int localy;
  int tent;
};

struct mundo{
  int N_herois;
  struct s_heroi *herois;
  int N_bases;
  struct base *bases;
  int N_missoes;
  struct missao*missoes;
  int N_habilidades;
  int Tam_Mundox;
  int Tam_Mundoy;
  int Relogio;
  int ev_trat;
  int miss_cumpr;
  int min_miss;
  int max_miss;
  int num_mortos;
};

int aleat (int min, int max);

void iniciarMundo(struct mundo *mundo_ini);

void iniciarHabilidade(struct s_heroi* heroi, int numHabilidades);

void iniciarHabilidadeMissao(struct missao* missao);

struct s_heroi *iniciarHerois(struct mundo * mundo_ini);

struct base *iniciarBase(struct mundo *mundo_ini);

struct missao *iniciarMissao(struct mundo *mundo_ini);

void agendarevento(struct fprio_t *LEF, int tipo, int T, int H, int B, int M);

void ev_ini_herois(struct s_heroi *heroi, struct fprio_t *LEF);

void ev_ini_missoes(struct missao *missao, struct fprio_t *LEF);

void ev_fim_do_mundo(struct fprio_t *LEF);

void Chega(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF);

void Espera(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF);

void Desiste(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF);

void Avisa(int instante, struct mundo *M,  int B, struct fprio_t *LEF);

void Entra(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF);

void Sai(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF);

int DistanciaCart(int atualx, int atualy, int destx, int desty);

void Viaja(int instante, struct mundo *M, int H, int B, struct fprio_t *LEF);

void Morre(int instante, struct mundo *M, int H, int B, int missao, struct fprio_t *LEF);

void Missao(int instante, struct mundo *M, int id_missao, struct fprio_t *LEF);

void Fim(int instante, struct mundo *M);