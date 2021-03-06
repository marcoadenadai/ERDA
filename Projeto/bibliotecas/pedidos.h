#ifndef PEDIDOS_H_INCLUDED
#define PEDIDOS_H_INCLUDED
#include "items.h"
/*ESTA BIBLIOTECA SOMENTE DEFINE A ESTRUTURA DOS PEDIDOS E IMPLEMENTA SEU FUNCIONAMENTO EM FILA
  EH IMPORTANTE LEMBRAR QUE ELA NÃO IMPLEMENTA O FUNCIONAMENTO REAL DOS PEDIDOS NO SISTEMA!.*/

typedef enum Reg { centro = 1, sul = 2, leste = 3, norte = 4, oeste = 5 } Regiao;

typedef struct pedido {
    unsigned int num;              //id do pedido
    Regiao regiao;                 //regiao do pedido
    unsigned int tempo;            //tempo de entrega
    float v_pedido;                //valor total do pedido, calculado automaticamente pelo sistema
    Itens* I;                      // PILHA de ITENS DO PEDIDO!
    struct pedido* prox;
} pedido;

typedef struct pedidos {
    pedido* ini;
    pedido* fim;
} Pedidos; //ATENCAO, OS PEDIDOS SERAO UMA FILA!!!

//  verifica se a fila de pedidos está vazia
int vazia_Pedidos(Pedidos* f) {
    if (f->ini == NULL) return 1;
    return 0;
}

//  inicializa a pilha
Pedidos* inicializa_Pedidos() {
    Pedidos* f = (Pedidos*)malloc(sizeof(Pedidos));    //alocando espaço da struct
    f->ini = f->fim = NULL;
    return f;
}

//  insere novo pedido "info" no fim da fila
void insere_Pedidos(Pedidos* P, pedido info) {
    pedido* novo = (pedido*)malloc(sizeof(pedido));
    novo->num = info.num;
    novo->regiao = info.regiao;
    novo->tempo = info.tempo;
    novo->v_pedido = info.v_pedido;
    novo->I = info.I;
    novo->prox = NULL;
    if (P->fim)//se o fim nao for vazio, insere no fim
        P->fim->prox = novo;

    P->fim = novo;
    if (!P->ini)//se a fila estava vazia antes
        P->ini = P->fim;
}

//  retira do inicio da fila de Pedidos, "atende"
pedido retira_Pedidos(Pedidos* P) {
    pedido ret;
    if (vazia_Pedidos(P)) {
        printf("Fila de Pedidos vazia, nao eh possivel retirar.\n");
        exit(0);
    }
    ret.num = P->ini->num;
    ret.regiao = P->ini->regiao;
    ret.tempo = P->ini->tempo;
    ret.v_pedido = P->ini->v_pedido;
    ret.I = P->ini->I;
    ret.prox = NULL;

    pedido* rmv = P->ini;
    P->ini = P->ini->prox;
    free(rmv);

    if (!P->ini) //fila ficou vazia
        P->fim = NULL;
    return ret;
}

int GetTempoEspera(Pedidos* P, int id); //Declaração pra utilização no Imprime_Pedidos

//  percorre a fila, imprimindo todos os valores de todos os Itens e seu tempo total de entrega
void imprime_Pedidos(Pedidos* P) {
    pedido* aux;
    printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
    for (aux = P->ini; aux; aux = aux->prox) {
        printf("Pedido #%d\n", aux->num);
        printf("Regiao: ");
        switch (aux->regiao) {
        case centro:
            printf("Centro\n");
            break;
        case sul:
            printf("Zona Sul\n");
            break;
        case leste:
            printf("Zona Leste\n");
            break;
        case norte:
            printf("Zona Norte\n");
            break;
        case oeste:
            printf("Zona Oeste\n");
            break;
        default:
            printf("?????\n");
            break;
        }
        printf("Tempo de Entrega: %d\n", GetTempoEspera(P, aux->num));    
        printf("Valor do Pedido: R$%.2f\n", aux->v_pedido);
        imprime_Itens(aux->I);
        printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
    }
}

//  percorre a fila, e libera memoria (free)
void libera_Pedidos(Pedidos* P) {
    pedido* rmv, * aux = P->ini;
    while (aux) {
        rmv = aux;
        libera_Itens(aux->I);
        aux = aux->prox;
        free(rmv);
    }
    free(P);
}

//  Percorre a fila, encontra todos os tempos de cada pedido, e os soma
int GetTempoEspera(Pedidos* P, int id) {
    pedido* aux;
    int ret = 0;
    //enquanto aux não for NULL, e nem id for igual ao id do pedido,para somar todos atraz do pedido alvo
    for (aux = P->ini; aux && aux->num <= id; aux = aux->prox) {
        ret += aux->tempo;
    }
    return ret + 15; //+15 do frete
}

#endif
