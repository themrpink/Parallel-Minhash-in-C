#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <stdint.h>
#include "time_test.c"
#include "hash_FNV_1.c"
#include "shingle_extract_omp.c"



#define PRIMES_SIZE 199
#define K_SHINGLE 63
#define MAX_LONG_LONG 0xcbf29ce484222325LL

/*
COMPILE OMP: gcc -g -Wall -fopenmp -o  FILE FILE.C -lm

*/

int main(int argc, char *argv[]){

    unsigned long long *signatures;
    long tot_shingles;
    char **shingles;

    if (argc!=4){
        printf("\ntest_main_omp [nome file] [numero di threads] [numero di cicli]\n\n");
        exit(1);
    }

    FILE * fp = fopen(argv[1], "r");
    int thread_count = atoi(argv[2]);
    omp_set_num_threads(thread_count);
    int iter = atoi(argv[3]);
    unsigned long long *signatures2 = malloc(200*sizeof(unsigned long long));

    for(int i=0; i<iter; i++){
        
        shingles = get_shingles_serial(fp, &tot_shingles, thread_count);
        signatures = get_signatures_s(shingles, tot_shingles, thread_count);
        for(int k=0; k<tot_shingles;k++){
            free(shingles[k]);
        }
        free(shingles);
        memcpy(signatures2, signatures, 200*8);
        free(signatures);

        /////////////////////////////////////////////////////////////////
        
        shingles = get_shingles(fp, &tot_shingles, thread_count);
        signatures = get_signatures(shingles, tot_shingles, thread_count); 
        for(int k=0; k<tot_shingles;k++){
            free(shingles[k]);
        }
        free(shingles);
        for(int i=0; i<200; i++){
            if(signatures[i]!=signatures2[i]){
                printf("la versione parallela sbaglia a calcolare le signatures: %llu != %llu in posizione %d\n", signatures[i],signatures2[i],i);
                exit(0);
            }
        }
        if(i==iter-1)
            printf("\nok, versione parallela e seriale producono le stesse signatures\n");
        free(signatures);


 }  

    fclose(fp);
    exectimes(thread_count, Any, iter);

    return 0;

}

