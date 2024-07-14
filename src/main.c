#include <stdio.h>
#include <stdint.h>
#include "conjuntos.h"

uint32_t boundDado(uint32_t *escolhidos, uint32_t n_candidatos) {
        uint32_t num_escolhidos = 0;
        for (uint32_t i = 0; i < n_candidatos; i++) {
                if (escolhidos[i] == 1) {
                        num_escolhidos++;
                }
        }
}

void zerarVetorUint32(uint32_t *vetor, uint32_t n) {
        for (uint32_t i = 0; i < n; i++) {
                vetor[i] = 0;
        }
}

// Retorna 1 se todos forem 1
uint32_t validaGrupo(uint32_t *grupos, uint32_t n_grupos) {
        for (uint32_t i = 0; i < n_grupos; i++) {
                if (grupos[i] == 0) {
                        return 0;
                }
        }
        return 1;
}


uint32_t validaEscolhidos(uint32_t *escolhidos, conjunto **candidatos, uint32_t n_candidatos, uint32_t n_grupos) {
        uint32_t *grupos = criarVetorUint32(n_grupos);
        zerarVetorUint32(grupos, n_grupos);

        for (uint32_t i = 0; i < n_candidatos; i++) {
                // Se o candidato for escolhido 
                if (escolhidos[i] == 1) {
                        conjunto *candidato = candidatos[i];
                        for (uint32_t j = 0; j < candidato->tam; j++) {
                                grupos[candidato->elementos[j] - 1] = 1;
                        }
                        if (validaGrupo(grupos, n_grupos)) {
                                free(grupos);
                                return 1;
                        }
                }
        }
        free(grupos);
        return 0;
}

uint32_t comissaoRepresentativa(uint32_t *escolhidos, conjunto **candidatos, uint32_t n_candidatos, uint32_t *grupos, uint32_t n_grupos, uint32_t i) {

        if (i >= n_candidatos) {
                if (validaEscolhidos(escolhidos, candidatos, n_candidatos, n_grupos)) {
                        printf("ESCOLHA VALIDA!\n");
                        for (uint32_t i = 0; i < n_candidatos; i++) {
                                printf("%u ", escolhidos[i]);
                        }
                        printf("\n\n");

                        return 1;
                }
                for (uint32_t i = 0; i < n_candidatos; i++) {
                        printf("%u ", escolhidos[i]);
                }
                printf("\n");
                return 0;
        }



        escolhidos[i] = 0;
        comissaoRepresentativa(escolhidos, candidatos,  n_candidatos, grupos, n_grupos, i + 1);

        escolhidos[i] = 1;
        comissaoRepresentativa(escolhidos, candidatos,  n_candidatos, grupos, n_grupos, i + 1);

        return 0;
}

int main() {
        uint32_t num_grupos, num_candidatos;

        scanf("%u %u", &num_grupos, &num_candidatos);
        //printf("grupos = %u, candidatos = %u\n", num_grupos, num_candidatos);

        conjunto *candidatos[num_candidatos];

        uint32_t tam_atual;
        for (uint32_t i = 0; i < num_candidatos; i++) {
                scanf("%u", &tam_atual);
                candidatos[i] = criarConjunto(tam_atual);
                for (uint32_t j = 0; j < tam_atual; j++) {
                        scanf("%u", &candidatos[i]->elementos[j]);
                }
        }

        uint32_t *escolhidos = criarVetorUint32(num_candidatos);
        uint32_t *grupos = criarVetorUint32(num_grupos);

        zerarVetorUint32(escolhidos, num_candidatos);
        zerarVetorUint32(grupos, num_grupos);

        comissaoRepresentativa(escolhidos, candidatos, num_candidatos, grupos, num_grupos, 0);

        for (uint32_t i = 0; i < num_candidatos; i++) {
                imprimirConjunto(candidatos[i]);
                destruirConjunto(candidatos[i]);
        }

        free(escolhidos);
        free(grupos);

        return EXIT_SUCCESS;
}
