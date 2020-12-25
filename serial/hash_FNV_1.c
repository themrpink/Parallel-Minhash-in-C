#include "hash_FNV_1.h"

int hash_FNV_1a(char *shingle, long long unsigned *hash, int lenght){

    long long unsigned FNV_offset_basis = MAX_LONG_LONG;
    long long unsigned prime = 1099511628211;

    *hash = FNV_offset_basis;

    for(int i=0; i<lenght; i++){
        (*hash) *= prime;
        (*hash) ^= shingle[i];
    }

    return 0;
}




long long unsigned* get_signatures(char **shingles, long long tot_shingles){
    
    long long unsigned hash=0;
    long long unsigned minhash=MAX_LONG_LONG;
   // printf("tot : %lld\n", tot_shingles);
    long long unsigned *hashed_shingles = (long long unsigned *)malloc(tot_shingles*sizeof(long long unsigned));

   // printf("tot : %lld\n", tot_shingles);
    long long unsigned *signatures;
//    printf("tot : %lld\n", tot_shingles);
    //qua vengono salvate le signatures
    signatures = (long long unsigned *)malloc(200*sizeof(long long unsigned));

    for(long long j=0; j < tot_shingles; j++){

        //lancia la prima funzione di hash su ogni shingle 
        hash_FNV_1a(shingles[j], &hash, K_SHINGLE);
        hashed_shingles[j] = hash;
        
        if(hash < minhash)
            minhash = hash;
    }

    *signatures=minhash;

    //applica la funzione di hash con PRIMES_SIZE valori diversi su tutte gli hashed_shingles, e ricava i minhash
    for(int i=0; i<PRIMES_SIZE; i++){
    
        minhash = MAX_LONG_LONG;
  
        for(long long j=0; j<tot_shingles; j++){

            hash = hashed_shingles[j] ^ rands[i];

            if(hash < minhash)
                minhash = hash;
        }     
        
        *(signatures+i+1)=minhash;
        //printf("%" PRIu64 "\n", *(signatures+i));
    }
    free(hashed_shingles);
    return signatures;
}



