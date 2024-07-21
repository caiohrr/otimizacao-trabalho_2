#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include "conjuntos.h"

#define FLAG_VIABILIDADE 'f'
#define FLAG_OTIMALIDADE 'o'
#define FLAG_FUNCAO_LIMITANTE 'a'

typedef struct flags_t {
        uint8_t viabilidade:1;
        uint8_t otimalidade:1;
        uint8_t funcao_limitante:1;
} flags_t;

void insertionSortReverse(int32_t arr[], int32_t n) {
        for (int32_t i = 1; i < n; i++) {
                int32_t key = arr[i];
                int32_t j = i - 1;

                // Move elements of arr[0..i-1], that are less than key,
                // to one position ahead of their current position
                while (j >= 0 && arr[j] < key) {
                        arr[j + 1] = arr[j];
                        j = j - 1;
                }
                arr[j + 1] = key;
        }
}

void imprimirVetorInt32(int32_t *vetor, int32_t n) {
        for (int32_t i = 0; i < n; i++) {
                printf("%d ", vetor[i]);
        }
        printf("\n");
}

void zerarVetorInt32(int32_t *vetor, int32_t n) {
        for (int32_t i = 0; i < n; i++) {
                vetor[i] = 0;
        }
}

// Retorna 1 se todos forem 1
int32_t validaGrupo(int32_t *grupos, int32_t n_grupos) {
        for (int32_t i = 0; i < n_grupos; i++) {
                if (grupos[i] == 0) {
                        return 0;
                }
        }
        return 1;
}


int32_t validaEscolhidos(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos, int32_t *grupos_restantes) {

//#ifdef DEBUG
//        //printf("VALIDA ESCOLHIDOS!\n");
//#endif

        int32_t *grupos = criarVetorInt32(n_grupos);
        zerarVetorInt32(grupos, n_grupos);
        int32_t qtd_escolhidos = 0;
        *grupos_restantes = n_grupos;

        //printf("Validando escolhidos...\n");
        //imprimirVetorInt32(escolhidos, n_candidatos);
        for (int32_t i = 0; i < n_candidatos; i++) {
                // Se o candidato for escolhido 
                if (escolhidos[i] == 1) {
                        //printf("O cadidato %d foi escolhido", i + 1);
                        qtd_escolhidos++;
                        conjunto *candidato = candidatos[i];
                        for (int32_t j = 0; j < candidato->tam; j++) {
                                if (grupos[candidato->elementos[j] - 1] != 1) {
                                        *grupos_restantes -= 1;
                                }
                                grupos[candidato->elementos[j] - 1] = 1;
                        }

                        //printf("Grupos apos escolher o candidato %d: ", i + 1);
                        //imprimirVetorInt32(grupos, n_grupos);
                        //if (validaGrupo(grupos, n_grupos)) {
                        //        free(grupos);
                        //        return qtd_escolhidos;
                        //}
                }
        }
        if (validaGrupo(grupos, n_grupos)) {
                free(grupos);
                return qtd_escolhidos;
        }
        free(grupos);
        return -qtd_escolhidos;
}

int32_t boundDado(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos) {

//#ifdef DEBUG
//        printf("BOUND DADO!\n");
//#endif

        int32_t grupos_restantes = 0;
        int32_t num_escolhidos = validaEscolhidos(escolhidos, candidatos, n_candidatos, n_grupos, &grupos_restantes);
        if (num_escolhidos < 0) {
#ifdef DEBUG
                printf("\nEscolhidos: ");
                imprimirVetorInt32(escolhidos, n_candidatos);
                printf("bound = %d\n", -num_escolhidos + 1);
#endif
                return -num_escolhidos + 1;
        }

#ifdef DEBUG
        printf("\nEscolhidos: ");
        imprimirVetorInt32(escolhidos, n_candidatos);
        printf("bound = %d\n", num_escolhidos);
#endif

        return num_escolhidos;

}

// Divisao do tamanho da soma do tamanho do conjunto de todos os nao escolhidos, dividido pela quantidade de candidatos nao escolhidos
int32_t boundNova(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos, int32_t i) {

//#ifdef DEBUG
//        printf("BOUND NOVA!\n");
//#endif

        int32_t bound = 0, grupos_restantes = 0;
        int32_t num_escolhidos = validaEscolhidos(escolhidos, candidatos, n_candidatos, n_grupos, &grupos_restantes);
        if (num_escolhidos < 0) {
                num_escolhidos *= -1;
        }

        int32_t *tamCandidatos = criarVetorInt32(n_candidatos);
        for (int32_t j = 0; j < i; j++) {
                tamCandidatos[j] = 0;
        }
        for (int32_t j = i; j < n_candidatos; j++) {
                tamCandidatos[j] = candidatos[j]->tam;

        }

#ifdef DEBUG
        printf("\ni = %d, grupos_restantes = %d\n Escolhidos(%d): ", i, grupos_restantes, num_escolhidos);
        imprimirVetorInt32(escolhidos, n_candidatos);
        printf("tamCandidatos: ");
        imprimirVetorInt32(tamCandidatos, n_candidatos);

#endif
        insertionSortReverse(tamCandidatos, n_candidatos);
#ifdef DEBUG
        printf("tamCandidatos (ordenados): ");
        imprimirVetorInt32(tamCandidatos, n_candidatos);
#endif

        bound = num_escolhidos;
        //bound = 0;
        int32_t j = 0;
        while (grupos_restantes > 0) {
                grupos_restantes -= tamCandidatos[j++];
                bound++;
        }

#ifdef DEBUG
        printf("bound = %d\n", bound);
        //printf("num_escolhidos = %d, grupos_restantes = %d, maxCandidato = %d\n", num_escolhidos, grupos_restantes, maxCandidato);
        //printf("bound = %d\n", bound);
#endif

        free(tamCandidatos);
        return bound;
}

void imprimirResposta(int32_t *escolhidos, int32_t n_candidatos) {
        for (int32_t i = 0; i < n_candidatos; i++) {
                if (escolhidos[i] == 1) {
                        printf("%d ", i + 1);
                }
        }
        printf("\n");
}

int32_t nodos = 0;
flags_t flags = {1, 1, 1};

int32_t comissaoRepresentativaRec(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos, int32_t i, int32_t *minEscolhidos, int32_t *definitivos) {
        nodos ++;
        //imprimirVetorInt32(escolhidos, n_candidatos);

        int32_t qtd_escolhidos;
        int32_t grupos_restantes = 0;
        if (flags.viabilidade == 1) {
                qtd_escolhidos = validaEscolhidos(escolhidos, candidatos, n_candidatos, n_grupos, &grupos_restantes);
        } else {
                qtd_escolhidos = 0;
        }

        if (qtd_escolhidos > 0 || i >= n_candidatos) {
                //printf("qtd_escolhidos = %d, minEscolhidos = %d\n", qtd_escolhidos, *minEscolhidos);
                //printf("Nodo = %d\n", nodos);
                if (flags.viabilidade == 0 && i >= n_candidatos) {
                        int32_t tmp_escolhidos = validaEscolhidos(escolhidos, candidatos, n_candidatos, n_grupos, &grupos_restantes);
                        //int32_t tmp_escolhidos = 100;
                        //printf("V: tmp_escolhidos = %d, minEscolhidos = %d\n", tmp_escolhidos, *minEscolhidos);
                        if (tmp_escolhidos > 0 && tmp_escolhidos < *minEscolhidos) {
                                //printf("COPIOU DEFINITIVOS!!!!!!!!!\n");
                                //imprimirVetorInt32(escolhidos, n_candidatos);
                                *minEscolhidos = tmp_escolhidos;
                                memcpy(definitivos, escolhidos, n_candidatos * sizeof(int32_t));
                                return *minEscolhidos;
                        }
                }

                if (qtd_escolhidos > 0 && qtd_escolhidos < *minEscolhidos) {
                        //printf("ENTROU!\n");
                        *minEscolhidos = qtd_escolhidos;
#ifdef DEBUG
                        printf("ATUALIZANDO MIN!! Escolhidos: ");
                        imprimirVetorInt32(escolhidos, n_candidatos);
#endif
                        memcpy(definitivos, escolhidos, n_candidatos * sizeof(int32_t));
                        return *minEscolhidos;
                }
                return *minEscolhidos;
        }

        // Cálculo do bound
        if (flags.otimalidade == 1) {
                if (flags.funcao_limitante == 1) {
                        int32_t bound = boundNova(escolhidos, candidatos, n_candidatos, n_grupos, i);
                        if (bound >= *minEscolhidos) {
#ifdef DEBUG
                                printf("PODA! Bound = %d, minEscolhidos = %d\n", bound, *minEscolhidos);
                                imprimirVetorInt32(escolhidos, n_candidatos);
#endif
                                return *minEscolhidos;  // Poda
                        }
                } else {
                        int32_t bound = boundDado(escolhidos, candidatos, n_candidatos, n_grupos);
                        if (bound >= *minEscolhidos) {
#ifdef DEBUG
                                printf("PODA! Bound = %d, minEscolhidos = %d\n", bound, *minEscolhidos);
                                imprimirVetorInt32(escolhidos, n_candidatos);
#endif
                                return *minEscolhidos;  // Poda
                        }
                }
        }

        // Não escolher o candidato atual
        escolhidos[i] = 1;
        comissaoRepresentativaRec(escolhidos, candidatos, n_candidatos, n_grupos, i + 1, minEscolhidos, definitivos);

        // Escolher o candidato atual
        escolhidos[i] = 0;
        comissaoRepresentativaRec(escolhidos, candidatos, n_candidatos, n_grupos, i + 1, minEscolhidos, definitivos);

        return *minEscolhidos;
}

int32_t comissaoRepresentativa(conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos) {

        int32_t *definitivos = criarVetorInt32(n_candidatos);
        int32_t *escolhidos = criarVetorInt32(n_candidatos);

        zerarVetorInt32(escolhidos, n_candidatos);
        zerarVetorInt32(definitivos, n_candidatos);

        int32_t minEscolhidos = INT_MAX;

        if (flags.funcao_limitante == 0) {
                printf("Utilizando a função bound do professor\n");
        } else {
                printf("Utilizando uma nova função bound\n");
        }

        int32_t resultado = comissaoRepresentativaRec(escolhidos, candidatos, n_candidatos, n_grupos, 0, &minEscolhidos, definitivos);
        //printf("Comissao representativa para %d candidatos e %d grupos\n", n_candidatos, n_grupos);

        if (resultado == INT_MAX) {
                printf("Inviavel\n");
        } else {
                //printf("Número mínimo de candidatos escolhidos: %d\n", resultado);
                //imprimirVetorInt32(definitivos, n_candidatos);
                imprimirResposta(definitivos, n_candidatos);
        }
        printf("Nodos percorridos = %d\n", nodos);
        //printf("RESPOSTA:\n");
        //printf("\n");


        free(definitivos);
        free(escolhidos);
        return resultado;
}

int main(int argc, char *argv[]) {

        int32_t opt;
        int32_t num_grupos, num_candidatos;

        while((opt = getopt(argc, argv, "foa")) != -1) {
                switch(opt) {
                        case FLAG_VIABILIDADE:
                                flags.viabilidade = 0;
                                break;
                        case FLAG_OTIMALIDADE:
                                flags.otimalidade = 0;
                                break;
                        case FLAG_FUNCAO_LIMITANTE:
                                flags.funcao_limitante = 0;
                                break;
                }
        }
        //printf("flags v = %u o = %u fl = %u\n", flags.viabilidade, flags.otimalidade, flags.funcao_limitante);
        scanf("%d %d", &num_grupos, &num_candidatos);

#ifdef DEBUG
        printf("grupos = %u, candidatos = %u\n", num_grupos, num_candidatos);
#endif

        conjunto *candidatos[num_candidatos];

        int32_t tam_atual;
        for (int32_t i = 0; i < num_candidatos; i++) {
                scanf("%d", &tam_atual);
                candidatos[i] = criarConjunto(tam_atual);
                for (int32_t j = 0; j < tam_atual; j++) {
                        scanf("%d", &candidatos[i]->elementos[j]);
                }
        }

        int32_t *escolhidos = criarVetorInt32(num_candidatos);
        int32_t *grupos = criarVetorInt32(num_grupos);

        zerarVetorInt32(escolhidos, num_candidatos);
        zerarVetorInt32(grupos, num_grupos);

        //comissaoRepresentativa(escolhidos, candidatos, num_candidatos, grupos, num_grupos, 0);
        comissaoRepresentativa(candidatos, num_candidatos, num_grupos);

        for (int32_t i = 0; i < num_candidatos; i++) {
                //imprimirConjunto(candidatos[i]);
                destruirConjunto(candidatos[i]);
        }

        free(escolhidos);
        free(grupos);

        return EXIT_SUCCESS;
}
