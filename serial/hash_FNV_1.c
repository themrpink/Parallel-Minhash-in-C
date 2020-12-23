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



