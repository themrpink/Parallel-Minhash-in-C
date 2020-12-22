#include "hash_FNV_1.h"

int hash_FNV_1(char *shingle, int const prime_i, unsigned long long *hash){
    
    unsigned long long FNV_offset_basis = 0xcbf29ce484222325LL;

    if(prime_i >= 200){
        printf("hash prime index deve essere < 200, era invece %d\n", prime_i);
        return -1;
    }

    *hash = FNV_offset_basis;
    //s#pragma omp parallel for num_threads(4)
    for(int i=0; i<K_SHINGLE; i++){
        (*hash) *= primes[prime_i];
        (*hash) ^= shingle[i];
    }

    return 0;

}
