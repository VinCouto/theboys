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
  

  int tempo;
  int tipo;
  struct evento *evento;
  evento = malloc(sizeof(struct evento));
  if(evento == NULL)
    return 0;
  ev_ini_herois(mundo->herois,LEF);
  ev_fim_do_mundo(LEF);

  //eventos iniciais
  printf("dps de td parabens vc n é tao burro");
  // executar o laço de simulação
  while(mundo->Relogio <= T_FIM_DO_MUNDO){
    evento = fprio_retira(LEF,&tipo,&tempo);

    mundo->ev_trat++;
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
        break;

      case FIM:
        Fim(tempo, mundo);
        break;

      default:
        break;
    }
    mundo->Relogio++;
    free(evento);
  }

  // destruir o mundo
  return (0);
}

