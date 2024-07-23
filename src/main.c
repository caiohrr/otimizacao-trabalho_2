#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <time.h>
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

        int32_t *grupos = criarVetorInt32(n_grupos);
        zerarVetorInt32(grupos, n_grupos);
        int32_t qtd_escolhidos = 0;
        *grupos_restantes = n_grupos;

        for (int32_t i = 0; i < n_candidatos; i++) {
                // Se o candidato for escolhido 
                if (escolhidos[i] == 1) {
                        qtd_escolhidos++;
                        conjunto *candidato = candidatos[i];
                        for (int32_t j = 0; j < candidato->tam; j++) {
                                if (grupos[candidato->elementos[j] - 1] != 1) {
                                        *grupos_restantes -= 1;
                                }
                                grupos[candidato->elementos[j] - 1] = 1;
                        }
                }
        }
        if (validaGrupo(grupos, n_grupos)) {
                free(grupos);
                return qtd_escolhidos;
        }
        free(grupos);
        return -qtd_escolhidos;
}

// Calcula a função bound dada pelo professor
int32_t boundDado(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos) {
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

// Calcula a função bound nova, explicada no relatório
int32_t boundNova(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos, int32_t i) {

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
        int32_t j = 0;
        while (grupos_restantes > 0) {
                grupos_restantes -= tamCandidatos[j++];
                bound++;
        }

        #ifdef DEBUG
                printf("bound = %d\n", bound);
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

// Globais para calcular os nodos percorridos e guardar as flags setadas
uint64_t nodos = 0;
flags_t flags = {1, 1, 1};

// Função recursiva para resolver o problema
int32_t comissaoRepresentativaRec(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos, int32_t i, int32_t *minEscolhidos, int32_t *definitivos) {
        nodos ++;

        int32_t qtd_escolhidos;
        int32_t grupos_restantes = 0;
        if (flags.viabilidade == 1) {
                qtd_escolhidos = validaEscolhidos(escolhidos, candidatos, n_candidatos, n_grupos, &grupos_restantes);
        } else {
                qtd_escolhidos = 0;
        }

        if (qtd_escolhidos > 0 || i >= n_candidatos) {
                if (flags.viabilidade == 0 && i >= n_candidatos) {
                        int32_t tmp_escolhidos = validaEscolhidos(escolhidos, candidatos, n_candidatos, n_grupos, &grupos_restantes);
                        if (tmp_escolhidos > 0 && tmp_escolhidos < *minEscolhidos) {
                                *minEscolhidos = tmp_escolhidos;
                                memcpy(definitivos, escolhidos, n_candidatos * sizeof(int32_t));
                                return *minEscolhidos;
                        }
                }

                if (qtd_escolhidos > 0 && qtd_escolhidos < *minEscolhidos) {
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

        #ifdef DEBUG
                if (flags.funcao_limitante == 0) {
                        printf("Utilizando a função bound do professor\n");
                } else {
                        printf("Utilizando uma nova função bound\n");
                }
        #endif

        clock_t start, end;
        double cpu_time;
        
        start = clock();
        int32_t resultado = comissaoRepresentativaRec(escolhidos, candidatos, n_candidatos, n_grupos, 0, &minEscolhidos, definitivos);
        end = clock();

        cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;

        if (resultado == INT_MAX) {
                printf("Inviavel\n");
        } else {
                imprimirResposta(definitivos, n_candidatos);
        }

        fprintf(stderr, "Nós percorridos = %lu\n", nodos);
        fprintf(stderr, "Tempo (ms) = %f\n", cpu_time * 1000);

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

        comissaoRepresentativa(candidatos, num_candidatos, num_grupos);

        for (int32_t i = 0; i < num_candidatos; i++) {
                destruirConjunto(candidatos[i]);
        }

        free(escolhidos);
        free(grupos);

        return EXIT_SUCCESS;
}
