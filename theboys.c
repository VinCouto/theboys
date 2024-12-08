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
  struct mundo *mundo;
  mundo = malloc(sizeof(struct mundo));
  if(mundo == NULL)
    return 0;
  printf("dps do malloc");
  printf("antes do incializa\n");
  iniciarMundo(mundo);
  mundo->herois = iniciarHerois(mundo);
  printf("INICIALIZOU HEROI\n");
  mundo->bases = iniciarBase(mundo);
  printf("INICIALIZOU BASE\n");
  mundo->missoes = iniciarMissao(mundo);
  printf("INICIALIZOU MISSAO\n");
  struct fprio_t *LEF = fprio_cria();
  printf("CRIOU FILA\n");
  printf("BASE 0 ID = %d\n", mundo->bases[0].ID);
  printf("BASE 0 CAP = %d\n", mundo->bases[0].lotação);
  printf("BASE 0 local x %d y %d\n", mundo->bases[0].localx, mundo->bases[0].localy);
  
  int continua = 1;
  int tempo = 0;
  int tipo;
  struct evento *evento;
  ev_ini_herois(mundo->herois,LEF);
  ev_ini_missoes(mundo->missoes,LEF);
  ev_fim_do_mundo(LEF);

  //eventos iniciais
  printf("dps de td parabens vc n é tao burro");
  // executar o laço de simulação
  while(continua != 0){
    evento = fprio_retira(LEF,&tipo,&tempo);

    switch (tipo){
      case CHEGA:
        Chega(tempo,mundo,evento->heroi_ID,evento->base_ID,LEF);
        break;
      
      case ESPERA:
        Espera(tempo,mundo,evento->heroi_ID,evento->base_ID,LEF);
        break;
      
      case DESISTE:
        Desiste(tempo,mundo,evento->heroi_ID,evento->base_ID,LEF);
        break;

      case AVISA:
        Avisa(tempo, mundo, evento->base_ID,LEF);
        break;

      case ENTRA:
        Entra(tempo, mundo, evento->heroi_ID, evento->base_ID, LEF);
        break;

      case SAI:
        Sai(tempo, mundo, evento->heroi_ID, evento->base_ID, LEF);
        break;

      case VIAJA:
        Viaja(tempo, mundo, evento->heroi_ID, evento->base_ID, LEF);
        break;

      case MORRE:
        Morre(tempo, mundo, evento->heroi_ID, evento->base_ID, 
        evento->missao_ID,LEF);
        break;

      case MISSAO:
        Missao(tempo, mundo, evento->missao_ID, LEF);
        break;

      case FIM:
        continua = 0;
        Fim(tempo, mundo);
        break;
      default:
        break;
    }
    free(evento);
  }

  // destruir o mundo
  fprio_destroi(LEF);
  return (0);
}

