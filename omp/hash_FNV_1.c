#include "hash_FNV_1.h"

int hash_FNV_1(char *shingle, unsigned long long *hash){
    
    unsigned long long FNV_offset_basis = 0xcbf29ce484222325LL;
    unsigned long long prime = 1099511628211;

    *hash = FNV_offset_basis;
    //s#pragma omp parallel for num_threads(4)
    for(int i=0; i<K_SHINGLE; i++){
        (*hash) *= prime;
        (*hash) ^= shingle[i];
    }
    return 0;

}


void hash_fun(unsigned long long hashed_shingle, unsigned long long *hash, int i){

     *hash = hashed_shingle ^ rands[i];
}




int hash_FNV_200(char *shingle, int const prime_i, unsigned long long *hash){
    
    unsigned long long FNV_offset_basis = 0xcbf29ce484222325LL;

    if(prime_i >= 200){
        printf("hash prime index deve essere < 200, era invece %d\n", prime_i);
        return -1;
    }

    *hash = FNV_offset_basis;

    for(int i=0; i<K_SHINGLE; i++){
        (*hash) *= primes[prime_i];
        (*hash) ^= shingle[i];
    }

    return 0;

}
