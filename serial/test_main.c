#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "hash_FNV_1.c"
#include "shingle_extract.c"
//#include "documents_getters.c"
#include "test_main.h"
#include "get_similarities.c"

/*
COMPILE OMP: gcc -g -Wall -o  test_main test_main.c -lm

*/


int main(int argc, char *argv[]){

    long long unsigned *signatures;
    long long tot_shingles;
    char **shingles;
    //const char *nomeDirectory = *argv[1];
    
    //char *filename;
    char files[][50] = {"./docs/doc.txt", "./docs/doc2.txt", "./docs/2.txt",  "./docs/3", "./docs/4", "./docs/5", "./docs/6", "./docs/doclungo2.txt", "./docs/doclungo3.txt"};//={"doc.txt", "doc2.txt", "doclungo3.txt"};
   // char **files = (char **)malloc(100*sizeof(char*));
    int numberOfFiles=9;
    FILE * fp;
 
    //list_dir("docs",files,&numberOfFiles );

    struct sign_doc **files_sketches;
    files_sketches = (struct sign_doc**) malloc(numberOfFiles * sizeof(struct sign_doc*));

    for(int i=0; i<numberOfFiles; i++)
        files_sketches[i]=(struct sign_doc*)malloc(N_SIGNATURES*sizeof(struct sign_doc));

    
    for(int i=0; i < numberOfFiles; i++){

        fp = fopen(*(files+i), "r");
        if(fp==NULL)
            perror("Error") ;

        shingles = get_shingles(fp, &tot_shingles);

        signatures = get_signatures(shingles, tot_shingles);

        get_sketches(files_sketches[i], signatures, *(files+i));


        for(long long i=0; i<tot_shingles; i++)
            free(shingles[i]);
        free(shingles);

        fclose(fp);
    }

    find_similarity(numberOfFiles, files_sketches);

    return 0;

}



