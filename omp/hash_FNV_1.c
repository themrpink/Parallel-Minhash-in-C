
#include <omp.h>
#include "hash_FNV_1.h"
#include "time_test.h"


int hash_FNV_1a(char *shingle, unsigned long long *hash){

    unsigned long long FNV_offset_basis = MAX_LONG_LONG;
    unsigned long long prime = 1099511628211;

    *hash = FNV_offset_basis;
    
    for(int i=0; i<K_SHINGLE; i++){
        (*hash) *= prime;
        (*hash) ^= shingle[i];
    }
    return 0;
}


unsigned long long* get_signatures(char **shingles, long tot_shingles, int thread_count){

    double start;
    double end;
    unsigned long long hash;
    unsigned long long minhash = MAX_LONG_LONG;

    unsigned long long *hashed_shingles = (unsigned long long *)malloc(tot_shingles*sizeof(unsigned long long *));
    unsigned long long *signatures = (unsigned long long *)malloc(200*sizeof(unsigned long long *));

    start = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) reduction(min:minhash) 
        for(int j=0; j < tot_shingles; j++){
            hash_FNV_1a(shingles[j], hashed_shingles+j);
            if(hashed_shingles[j]<minhash)
                minhash=hashed_shingles[j];
        }
    *signatures = minhash;


    minhash = MAX_LONG_LONG;
    //omp_set_nested(2) ;
    //printf("neted: %d\n", omp_get_nested());
    #pragma omp parallel for num_threads(thread_count)  private(hash) reduction(min:minhash) //collapse(2)// private (hash)//private(hash)// reduction(min:minhash)// schedule(static,25) //private(hash) reduction(min:minhash)// threadprivate(minhash)
        for(int i=1; i<=PRIMES_SIZE; i++){ 
            minhash = MAX_LONG_LONG;  
            for(int j=0; j<tot_shingles; j++){
                hash = hashed_shingles[j] ^ rands[i];
                if(hash<minhash)
                    minhash=hash;
            }        
        *(signatures+i)=minhash;
        }

     /*

         int rank = omp_get_num_threads();

        for(int i=1; i<=PRIMES_SIZE; i++){
           minhash = MAX_LONG_LONG; 
            #pragma omp parallel for reduction(min:minhash) private(hash)
            for(int j=0; j<tot_shingles; j++){
                hash = hashed_shingles[j] ^ rands[i];
                if(hash<minhash)
                    minhash=hash;
            }      
                
        *(signatures+i)=minhash;
        }
     */

    end = omp_get_wtime();
    exectimes(end - start, Parallel_signatures,0);
    return signatures;
}




unsigned long long* get_signatures_s(char **shingles, long tot_shingles, int thread_count){

    double start;
    double end;
    unsigned long long hash;
    unsigned long long minhash = MAX_LONG_LONG; 

    unsigned long long *hashed_shingles = (unsigned long long *)malloc(tot_shingles*sizeof(unsigned long long *));
    unsigned long long *signatures = (unsigned long long *)malloc(200*sizeof(unsigned long long *));

    start = omp_get_wtime();

    for(int j=0; j < tot_shingles; j++){
        hash_FNV_1a(shingles[j], hashed_shingles+j);
        if(hashed_shingles[j]<minhash)
        minhash=hashed_shingles[j];
    }
    *signatures = minhash;


        for(int i=1; i<=PRIMES_SIZE; i++){
            minhash = MAX_LONG_LONG;
            for(int j=0; j<tot_shingles; j++){
                hash = hashed_shingles[j] ^ rands[i];
                    if(hash<minhash)
                        minhash=hash;
            }      
            *(signatures+i)=minhash;
        }

    end = omp_get_wtime(); 
    exectimes(end - start, Serial_signatures,0);
    return signatures;
}
