#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#include "hash_FNV_1.c"
#include "shingle_extract.c"

#define PRIMES_SIZE 200
#define K_SHINGLE 54
#define MAX_LONG_LONG 0xcbf29ce484222325LL

/*
COMPILE OMP: gcc -g -Wall -fopenmp -o  FILE FILE.C -lm

*/


int main(int argc, char *argv[]){

    fprintf(stdout, "%lu\n", (unsigned long)time(NULL)); 

    unsigned long long hash;
    unsigned long long minhash; 
    unsigned long long *signatures =  (unsigned long long *)malloc(200*sizeof(unsigned long long *));
    unsigned long long * hashed_shingles; 
    long size;
    long tot_shingles;
    char **shingles;
    double start;
    double end;


    FILE * fp = fopen(argv[1], "r");

    //dimensione del file
    fseek(fp, 0, SEEK_END); // seek to end of file
    size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET); // seek back to beginning of file


    tot_shingles = size - K_SHINGLE + 1;


    //alloca lo spazio per gli shingles
    shingles = malloc(tot_shingles * (sizeof(char*)));

    for(int i=0; i<tot_shingles; i++)
        shingles[i] = malloc(K_SHINGLE*(sizeof(char)));
    
 


    //estrae gli shingles
    shingle_extract_buf(fp, size, shingles);




    //qua vengono salvati gli shingles passati dalla funzione di hash, che poi verranno a loro volta passati alle altre funzioni di hash
    hashed_shingles = (unsigned long long *)malloc(tot_shingles*sizeof(unsigned long long *));


    for(int j=0; j<tot_shingles; j++){
        //lancia la prima funzione di hash su ogni shingle 
        hash_FNV_1(shingles[j], &hash);
        hashed_shingles[j] = hash;

        if(hash<minhash)
            minhash=hash;
    }

    signatures[0]=minhash;


    //applica la funzione di hash con PRIMES_SIZE valori diversi su tutte gli hashed_shingles, e ricava i minhash
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

    

    fclose(fp);
    for(int i=0; i<tot_shingles; i++)
        free(shingles[i]);
    free(shingles);
}


