#include "hash_FNV_1.h"
#include <omp.h>


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
    //int c =  tot_shingles / thread_count ;

    start = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) reduction(min:minhash) //schedule(static,c)

        for(int j=0; j < tot_shingles; j++){

                hash_FNV_1a(shingles[j], hashed_shingles+j);

                if(hashed_shingles[j]<minhash)
                    minhash=hashed_shingles[j];

        }

    *signatures = minhash;

    
    #pragma omp parallel for num_threads(thread_count)// schedule(static,25) //private(hash) reduction(min:minhash)// threadprivate(minhash)
        for(int i=1; i<=PRIMES_SIZE; i++){
           minhash = MAX_LONG_LONG; 
            #pragma omp parallel for num_threads(thread_count) private(hash) reduction(min:minhash) //schedule(static,c) //copyin(minhash) 

            for(int j=0; j<tot_shingles; j++){

                hash = hashed_shingles[j] ^ rands[i];

                if(hash<minhash)
                    minhash=hash;
            }      
                
        *(signatures+i)=minhash;
        }

    end = omp_get_wtime();
    printf("\n \n opm version parallel for \n f: get_signatures \n number of threads: %d\n time: %f\n",thread_count, end - start);

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

        //    if(j==tot_shingles-1)

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
    printf("\n \n opm version serial for \n f: get_signatures_s \n number of threads: %d\n time: %f\n",1, end - start);

    return signatures;
}
