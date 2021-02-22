#ifndef MINHASHPROJECT_HASH_FNV_1_H
#define MINHASHPROJECT_HASH_FNV_1_H
#define PRIMES_SIZE 199
#define N_SIGNATURES 200
#define K_SHINGLE 54
#define MAX_LONG_LONG_U 0xffffffffffffffffLLU
#define FNV_PRIME 1099511628211LLU
#define FNV_OFFSET_BASIS 14695981039346656037LLU


extern unsigned long long rands[];


/*
    versione originale della funzione di hash FNV_1a
*/
int hash_FNV_1a(char *shingle, long long unsigned *hash);


/*
    funzione che si limita ad eseguire lo XOR
*/
long long unsigned * get_signatures(char **shingles, long long tot_shingles);

#endif //MINHASHPROJECT_HASH_FNV_1_H
