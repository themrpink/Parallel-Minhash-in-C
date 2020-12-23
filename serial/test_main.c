#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "hash_FNV_1.c"
#include "shingle_extract.c"



/*
COMPILE OMP: gcc -g -Wall -o  test_main test_main.c -lm

*/


int main(int argc, char *argv[]){

    uint64_t *signatures;
    long tot_shingles;
    char **shingles;

   
    FILE * fp = fopen(argv[1], "r");

    
    //ottiene gli shingles del documento
    shingles = get_shingles(fp, &tot_shingles);



    //ottiene le signatures del documento
    signatures = get_signatures(shingles, tot_shingles);
 

    for(int i=0; i<200; i++){
        printf("%d %" PRIu64 "\n", i,*(signatures+i));
    
    }


    return 0;
}