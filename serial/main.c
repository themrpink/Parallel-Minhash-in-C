//
// Created by dani2612 on 19/12/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "main.h"
#include "shingle_extract.h"
#include "documents_getters.h"





//folder e ricerca dei file
int main(int argc, char *argv[]){

    unsigned long long hash;
    unsigned long long minhash;
    long size;
    long tot_shingles;
    char **shingles;


    if (argc!=1){
        exit(EXITARGUMENTFAIL);
    }

    //for file in files
    //lettura file and tokenize



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

