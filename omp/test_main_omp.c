#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <stdint.h>
#include "hash_FNV_1.c"
#include "shingle_extract_omp.c"

#define PRIMES_SIZE 200
#define K_SHINGLE 54
#define MAX_LONG_LONG 0xcbf29ce484222325LL

/*
COMPILE OMP: gcc -g -Wall -fopenmp -o  FILE FILE.C -lm

*/


int main(int argc, char *argv[]){

    unsigned long long hash;
    unsigned long long minhash; 
    unsigned long long *signatures =  (unsigned long long *)malloc(200*sizeof(unsigned long long *));
    unsigned long long * hashed_shingles; 
    long size;
    long tot_shingles;
    char **shingles;
    double start;
    double end;

    if (argc!=3){
        printf("test_main_omp [nome file] [numero di threads]");
        exit(1);
    }

    int thread_count = atoi(argv[2]);
    printf("numero di threads: %d\n", thread_count);

    FILE * fp = fopen(argv[1], "r");
    fseek(fp, 0, SEEK_END); // seek to end of file
    size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET); // seek back to beginning of file

    tot_shingles = size - K_SHINGLE + 1;
    
    //alloca lo spazio per gli shingles
    shingles = malloc(tot_shingles * (sizeof(char*)));

    for(int i=0; i<tot_shingles; i++){
        shingles[i] = malloc(K_SHINGLE*(sizeof(char)));
    }



/*
    tempi di esecuzione dello shingle extract seriale e parallelo

*/
    fseek(fp, 0, SEEK_SET);
    start = omp_get_wtime();
    shingle_extract_buf_r(fp, size, thread_count, shingles);
    end = omp_get_wtime();
    printf("\n \n opm version parallel for \n f: shingle_extract_buf_r \n number of threads: %d\n time: %f\n",thread_count, end - start);



/*
    tempi di esecuzione del minhash seriale
*/
    fseek(fp, 0, SEEK_SET);
    start = omp_get_wtime();
    shingle_extract_buf(fp, size, thread_count, shingles);
    end = omp_get_wtime();
    printf("\n\n serial version simple loop\n f: shingle_extract_buf \n number of threads: %d\n time: %f\n", 1 ,  end - start);



    hashed_shingles = (unsigned long long *)malloc(tot_shingles*sizeof(unsigned long long *));
    start = omp_get_wtime();

    for(int j=0; j<tot_shingles; j++){
        //lancia la prima funzione di hash su ogni shingle 
        hash_FNV_1(shingles[j], &hash);
        hashed_shingles[j] = hash;

        if(hash<minhash)
            minhash=hash;
    }

    signatures[0]=minhash;

    //applica ogni funzione di hash ogni volta su tutti gli shingle, ricava i minhash
   // #pragma omp parallel for num_threads(2)
    for(int i=1; i<PRIMES_SIZE; i++){

        minhash = MAX_LONG_LONG;
      
        for(int j=0; j<tot_shingles; j++){
            //lancia la funzione di hash su ogni shingle 
            hash_fun(hashed_shingles[j], &hash, i);
            if(hash<minhash)
                minhash=hash;
        }       
        signatures[i]=minhash;
    }
    
    end = omp_get_wtime();
    printf("\n\n serial version hash\n f: hash \n number of threads: %d\n time: %f\n", 1 ,  end - start);


    start = omp_get_wtime();
    //#pragma omp parallel for num_threads(thread_count)
    for(int i=0; i<PRIMES_SIZE; i++){

        minhash = MAX_LONG_LONG;
            
        for(int j=0; j<tot_shingles; j++){
            //lancia la funzione di hash su ogni shingle 
            hash_FNV_200(shingles[j], i, &hash);

            if(hash<minhash)
                minhash=hash;
        }
    }
    
    end = omp_get_wtime();
    printf("\n\n parallel version hash omp\n f: hash \n number of threads: %d\n time: %f\n",thread_count,  end - start);

   

    fclose(fp);
    for(int i=0; i<tot_shingles; i++)
        free(shingles[i]);
    free(shingles);


return 0;


}


