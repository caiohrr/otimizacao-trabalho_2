#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct conjunto {
        uint32_t tam;
        uint32_t *elementos;
} conjunto;

uint32_t *criarVetorUint32(uint32_t n);

conjunto *criarConjunto(uint32_t tam);

void destruirConjunto(conjunto *conj);

void imprimirConjunto(conjunto *conj);
