#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct conjunto {
        int32_t tam;
        int32_t *elementos;
} conjunto;

int32_t *criarVetorInt32(int32_t n);

conjunto *criarConjunto(int32_t tam);

void destruirConjunto(conjunto *conj);

void imprimirConjunto(conjunto *conj);
