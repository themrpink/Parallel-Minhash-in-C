#include "hash_FNV_1.h"

int hash_FNV_1a(char *shingle, uint64_t *hash){

    uint64_t FNV_offset_basis = MAX_LONG_LONG;
    uint64_t prime = 1099511628211;

    *hash = FNV_offset_basis;

    for(int i=0; i<K_SHINGLE; i++){
        (*hash) *= prime;
        (*hash) ^= shingle[i];
    }

    return 0;
}




uint64_t* get_signatures(char **shingles, long tot_shingles){
    
    uint64_t hash;
    uint64_t minhash; 
    uint64_t hashed_shingles[tot_shingles]; 
    uint64_t *signatures;
   
    //qua vengono salvate le signatures
    signatures = (uint64_t *)malloc(200*sizeof(uint64_t *));

    for(int j=0; j < tot_shingles; j++){

        //lancia la prima funzione di hash su ogni shingle 
        hash_FNV_1a(shingles[j], &hash);
        hashed_shingles[j] = hash;
        
        if(hash < minhash)
            minhash = hash;
    }

    *signatures=minhash;

    //applica la funzione di hash con PRIMES_SIZE valori diversi su tutte gli hashed_shingles, e ricava i minhash
    for(int i=0; i<PRIMES_SIZE; i++){
    
        minhash = MAX_LONG_LONG;
  
        for(int j=0; j<tot_shingles; j++){

            hash = hashed_shingles[j] ^ rands[i];

            if(hash < minhash)
                minhash = hash;
        }     
        
        *(signatures+i+1)=minhash;
        //printf("%" PRIu64 "\n", *(signatures+i));
    }
   
    return signatures;
}



