// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"
#include "mundo.h"
// seus #defines vão aqui


// minimize o uso de variáveis globais


// programa principal
int main (){
  // iniciar o mundo
  printf("antes do malloc");
  struct mundo *mundo_ini;
  mundo_ini = malloc(sizeof(struct mundo));
  if(mundo_ini == NULL)
    return 0;
  printf("dps do malloc");
  printf("antes do incializa\n");
  iniciarMundo(mundo_ini);
  mundo_ini->herois = iniciarHerois(mundo_ini);
  mundo_ini->bases = iniciarBase(mundo_ini);
  mundo_ini->missoes = iniciarMissao(mundo_ini);
  struct fprio_t *LEF = fprio_cria();
  printf("num herois %2d", N_HEROIS);
  Chega(0,mundo_ini,0,2,LEF);
  Chega(0,mundo_ini,2,2,LEF);
  Espera(0,mundo_ini,0,2,LEF);
  Desiste(1,mundo_ini,2,2,LEF);
  Avisa(1,mundo_ini,2,LEF);
  Entra(2,mundo_ini,0,3,LEF);
  Sai(3,mundo_ini,0,2,LEF);
  Viaja(3,mundo_ini,0,4,LEF);
  //eventos iniciais
  printf("dps de td parabens vc n é tao burro");
  // executar o laço de simulação

  // destruir o mundo
  Fim(mundo_ini);
  return (0);
}

