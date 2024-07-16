#include "conjuntos.h"

int32_t *criarVetorInt32(int32_t n) {
        int32_t *vetor = malloc(sizeof(int32_t) * n);
        if (vetor == NULL) {
                return NULL;
        }

        return vetor;
}

conjunto *criarConjunto(int32_t tam) {
        conjunto *conj = malloc(sizeof(conjunto));
        if (conj == NULL) {
                return NULL;
        }

        conj->tam = tam;
        conj->elementos = criarVetorInt32(tam);

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
        printf("%d:", conj->tam);
        for (int32_t i = 0; i < conj->tam; i++) {
                printf(" %d", conj->elementos[i]);
        }
        printf("\n");
}
