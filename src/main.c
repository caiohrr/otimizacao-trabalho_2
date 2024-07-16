#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include "conjuntos.h"

void imprimirVetorInt32(int32_t *vetor, int32_t n) {
        for (int32_t i = 0; i < n; i++) {
                printf("%i ", vetor[i]);
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


int32_t validaEscolhidos(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos) {
        int32_t *grupos = criarVetorInt32(n_grupos);
        zerarVetorInt32(grupos, n_grupos);
        int32_t qtd_escolhidos = 0;

        printf("Validando escolhidos...\n");
        imprimirVetorInt32(escolhidos, n_candidatos);
        for (int32_t i = 0; i < n_candidatos; i++) {
                // Se o candidato for escolhido 
                if (escolhidos[i] == 1) {
                        printf("O cadidato %d foi escolhido", i + 1);
                        qtd_escolhidos++;
                        conjunto *candidato = candidatos[i];
                        for (int32_t j = 0; j < candidato->tam; j++) {
                                grupos[candidato->elementos[j] - 1] = 1;
                        }
                        
                        printf("Grupos apos escolher o candidato %d: ", i + 1);
                        imprimirVetorInt32(grupos, n_grupos);
                        if (validaGrupo(grupos, n_grupos)) {
                                free(grupos);
                                return qtd_escolhidos;
                        }
                }
        }
        free(grupos);
        return -qtd_escolhidos;
}

int32_t boundDado(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos) {

        int32_t num_escolhidos = validaEscolhidos(escolhidos, candidatos, n_candidatos, n_grupos);
        if (num_escolhidos < 0) {
                return -num_escolhidos + 1;
        }
        return num_escolhidos;

        return 0;
}

int32_t nodos = 0;
int32_t comissaoRepresentativaRec(int32_t *escolhidos, conjunto **candidatos, int32_t n_candidatos, int32_t n_grupos, int32_t i, int32_t *minEscolhidos, int32_t *definitivos) {
        nodos ++;
        imprimirVetorInt32(escolhidos, n_candidatos);
        
        int32_t qtd_escolhidos = validaEscolhidos(escolhidos, candidatos, n_candidatos, n_grupos);
        if (qtd_escolhidos > 0 || i >= n_candidatos) {
                printf("qtd_escolhidos = %d, minEscolhidos = %d\n", qtd_escolhidos, *minEscolhidos);
                if (qtd_escolhidos > 0 && qtd_escolhidos < *minEscolhidos) {
                        *minEscolhidos = qtd_escolhidos;
                        printf("Escolhidos: ");
                        imprimirVetorInt32(escolhidos, n_candidatos);
                        memcpy(definitivos, escolhidos, n_candidatos * sizeof(int32_t));
                }
                return *minEscolhidos;
        }

        // Cálculo do bound
        int32_t bound = boundDado(escolhidos, candidatos, n_candidatos, n_grupos);
        if (bound >= *minEscolhidos) {
                printf("PODA! Bound = %d, minEscolhidos = %d\n", bound, *minEscolhidos);
                imprimirVetorInt32(escolhidos, n_candidatos);
                return *minEscolhidos;  // Poda
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
        int32_t resultado = comissaoRepresentativaRec(escolhidos, candidatos, n_candidatos, n_grupos, 0, &minEscolhidos, definitivos);
        printf("Comissao representativa para %d candidatos e %d grupos\n", n_candidatos, n_grupos);

        if (resultado == INT_MAX) {
                printf("Nenhuma solução válida encontrada.\n");
        } else {
                printf("Número mínimo de candidatos escolhidos: %d\n", resultado);
                imprimirVetorInt32(definitivos, n_candidatos);
        }
        printf("Nodos percorridos = %d\n", nodos);

        free(escolhidos);
        return resultado;
}

int main() {
        int32_t num_grupos, num_candidatos;

        scanf("%d %d", &num_grupos, &num_candidatos);
        //printf("grupos = %u, candidatos = %u\n", num_grupos, num_candidatos);

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
                imprimirConjunto(candidatos[i]);
                destruirConjunto(candidatos[i]);
        }

        free(escolhidos);
        free(grupos);

        return EXIT_SUCCESS;
}
