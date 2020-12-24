#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <stdint.h>
#include "hash_FNV_1.c"
#include "shingle_extract_omp.c"

#define PRIMES_SIZE 199
#define K_SHINGLE 54
#define MAX_LONG_LONG 0xcbf29ce484222325LL


/*
COMPILE OMP: gcc -g -Wall -fopenmp -o  FILE FILE.C -lm

*/

int main(int argc, char *argv[]){

    unsigned long long *signatures; //=  (unsigned long long *)malloc(200*sizeof(unsigned long long *));
    long tot_shingles;
    char **shingles;

    if (argc!=3){
        printf("test_main_omp [nome file] [numero di threads]");
        exit(1);
    }

    FILE * fp = fopen(argv[1], "r");

    int thread_count = atoi(argv[2]);
    omp_set_num_threads(thread_count);

    shingles = get_shingles(fp, &tot_shingles, thread_count);

    signatures = get_signatures(shingles, tot_shingles, thread_count);

    get_signatures_s(shingles, tot_shingles, thread_count);


    /*
    for(int i=0; i<200; i++)
        printf("%d \"%llu\"\n",i, signatures[i]);
    */
    
    fclose(fp);
    for(int i=0; i<tot_shingles; i++)
        free(shingles[i]);
    free(shingles);


return 0;


}







