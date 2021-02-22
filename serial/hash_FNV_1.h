#ifndef MINHASHPROJECT_HASH_FNV_1_H
#define MINHASHPROJECT_HASH_FNV_1_H
#define PRIMES_SIZE 199
#define N_SIGNATURES 200
#define K_SHINGLE 54
#define MAX_LONG_LONG_U 0xffffffffffffffffLLU
#define FNV_PRIME 1099511628211LLU
#define FNV_OFFSET_BASIS 14695981039346656037LLU


/*
    versione originale della funzione di hash FNV_1a, dalla quale si ottiene la prima signature di ogni documento
*/
int hash_FNV_1a(char *shingle, long long unsigned *hash);


/*
    funzione che si limita ad eseguire lo XOR tra i valori di hash degli shingles ottenuti con hash_FNV_1a() e 199 numeri random pre-generati, uguali per tutti i
    documenti. Di questi salva i minori, che compongono le 199+1 signatures di ogni ducumento.
*/
long long unsigned * get_signatures(char **shingles, long long tot_shingles);

#endif //MINHASHPROJECT_HASH_FNV_1_H
