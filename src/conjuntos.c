#include "conjuntos.h"

uint32_t *criarVetorUint32(uint32_t n) {
        uint32_t *vetor = malloc(sizeof(uint32_t) * n);
        if (vetor == NULL) {
                return NULL;
        }

        return vetor;
}

conjunto *criarConjunto(uint32_t tam) {
        conjunto *conj = malloc(sizeof(conjunto));
        if (conj == NULL) {
                return NULL;
        }

        conj->tam = tam;
        conj->elementos = criarVetorUint32(tam);

        if (conj->elementos == NULL) {
                return NULL;
        }

        return conj;
}

void destruirConjunto(conjunto *conj) {
        free(conj->elementos);
        free(conj);
}

void imprimirConjunto(conjunto *conj) {
        for (uint32_t i = 0; i < conj->tam; i++) {
                printf("%u ", conj->elementos[i]);
        }
        printf("\n");
}
