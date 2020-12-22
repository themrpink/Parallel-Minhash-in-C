#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <pthread.h>
#include <omp.h>

#include "hash_FNV_1.c"
#include "shingle_extract.c"

#define PRIMES_SIZE 200
#define K_SHINGLE 54
#define BANDS 20
#define MAX_LONG_LONG 0xcbf29ce484222325LL

/*
COMPILE OMP: gcc -g -Wall -fopenmp -o  FILE FILE.C

*/

struct doc_data {
    long doc_id;
    unsigned long long signature;
    int band;
};



int main(int argc, char *argv[]){

    fprintf(stdout, "%lu\n", (unsigned long)time(NULL)); 

    unsigned long long hash;
    unsigned long long minhash; 
    long size;
    long tot_shingles;
    char **shingles;
    double start;
    double end;


    FILE * fp = fopen(argv[1], "r");

    fseek(fp, 0, SEEK_END); // seek to end of file
    size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET); // seek back to beginning of file

    tot_shingles = size - K_SHINGLE + 1;

    //alloca lo spazio per gli shingles
    shingles = malloc(tot_shingles * (sizeof(char*)));

    for(int i=0; i<tot_shingles; i++)
        shingles[i] = malloc(K_SHINGLE*(sizeof(char)));
    
    /*
    //estrai gli shingles dal file
    start = omp_get_wtime();
    shingle_extract(fp, tot_shingles, shingles);
    end = omp_get_wtime();
    printf("\n\nelapsed time \n serial version with pipe from file\n: %f\n", end - start);
    */  

    fseek(fp, 0, SEEK_SET);
    start = omp_get_wtime();
    shingle_extract_buf(fp, size, shingles);
    end = omp_get_wtime();
    printf("\n\nelapsed time \n serial version witout pipe from buffer\n: %f\n", end - start);



    printf("\n\n\n\n############## minhash: ##############\n\n");
    //applica ogni funzione di hash su tutti gli shingle, ricava i minhash
    for(int i=0; i<PRIMES_SIZE; i++){
        minhash = MAX_LONG_LONG;

        for(int j=0; j<tot_shingles; j++){
            //lancia la funzione di hash su ogni shingle 
            hash_FNV_1(shingles[j], i, &hash);

            if(hash<minhash)
                minhash=hash;
        }
        
        printf("%llu \n", minhash);
    }

    

    fclose(fp);
    for(int i=0; i<tot_shingles; i++)
        free(shingles[i]);
    free(shingles);
}


