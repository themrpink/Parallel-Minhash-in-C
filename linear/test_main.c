#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include "hash_FNV_1.h"
#include "shingle_extract.h"

#define PRIMES_SIZE 200
#define K_SHINGLE 20
#define BANDS 20
#define MAX_LONG_LONG 0xcbf29ce484222325LL


struct doc_data {
    long doc_id;
    unsigned long long signature;
    int band;
};



int main(int argc, char *argv[]){

    unsigned long long hash;
    unsigned long long minhash; 
    long size;
    long tot_shingles;
    char **shingles;

    /*
    parte relativa allo struct:
        long id = 12341234;
        int count_bands=0;
        int cb = tot_shingles / BANDS;
        struct doc_data doc_struct[PRIMES_SIZE];
    */

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

    //estrai gli shingles dal file
    shingle_extract(fp, tot_shingles, shingles);



    
    printf("\n\n\n\n############## minhash: ##############\n\n");
    //applica ogni funzione di hash ogni volta su tutti gli shingle, ricava i minhash
    for(int i=0; i<PRIMES_SIZE; i++){

        minhash = MAX_LONG_LONG;

        for(int j=0; j<tot_shingles; j++){
            //lancia la funzione di hash su ogni shingle 
            hash_FNV_1(shingles[j], i, &hash);

            if(hash<minhash)
                minhash=hash;
        }

        /*
        //inserisci il tutto negli struct
        doc_struct[i].doc_id = id;
        doc_struct[i].signature=minhash;
        doc_struct[i].band=count_bands;
        if(cb==0){
            count_bands+=1;
            cb = tot_shingles / BANDS;
        }
        cb--;
        */
        
        printf("%llu \n", minhash);
    }

    fclose(fp);
    for(int i=0; i<tot_shingles; i++)
        free(shingles[i]);
    free(shingles);
}



/*
    
    ultima parte:
        abbiamo n minhash signatures.

        le dividiamo in band di 4 elementi, lo consideriamo minimo requisito 
        per essere simile

        poi prendiamo tutti i documenti

        ogni documento deve avere un id. noi dobbiamo creare questi struct {id, signature, band}
        questi dati vanno tutti ordinati insieme per shingle, in gruppi di bands

        quindi prima viene la band, poi la signatures.



*/




/*


se i documenti sono diversi per una parola, avremo K shingles diversi.
quindi quando li hasho ottengo minhash diversi



*/
