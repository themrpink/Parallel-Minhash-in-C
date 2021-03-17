#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include "hash_FNV_1.h"
#include "shingle_extract.h"
#include "documents_getters.h"
#include "tokenizer.h"
#include "get_similarities.h"
#include "time_test.h"
#include "string.h"


#define  EXITARGUMENTFAIL 20
#define  EXITNOFILEFOUND  30
#define COEFFICIENTE_SIMILARITA 0.75
//gcc -Wall -fopenmp -o main main.c documents_getters.c get_similarities.c hash_FNV_1.c  tokenizer.c shingle_extract.c time_test.c
//sudo perf stat -e L1-dcache-load-misses ./main "/home/stefano/Scrivania/Collegamento a uni/terzo anno/multicore 2020/progetto/git/minhash-multicore/docs" 16
//./main "/home/stefano/Scrivania/Collegamento a uni/terzo anno/multicore 2020/progetto/git/minhash-multicore/docs" 16

//funzione di supporto per il quick sort
int cmpfunc (const void * a, const void * b);


int main(int argc, char *argv[]) {

    int threads=atoi(argv[2]);
    printf("threads: %d\n", threads);
    omp_set_num_threads(threads);
    omp_set_nested(4);

    double start;
    double  end;
    start = omp_get_wtime();
    char *folderName = argv[1];
    char **files;
    int numberOfFiles = list_dir(folderName, &files);
    printf("numero di file: %d\n", numberOfFiles);
    if (numberOfFiles==0){
        exit(EXITNOFILEFOUND);
    }

    //il quick sort ordina i nomi dei file giusto per far funzionare il test sulle signatures
    //qsort(files, numberOfFiles, sizeof(files[0]),cmpfunc);

    long long unsigned **minhashDocumenti = (long long unsigned **)malloc(numberOfFiles*sizeof (long long unsigned *));

    

    #pragma omp parallel for schedule(static, 8)
    for (int i = 0; i < numberOfFiles; i++) {

            long fileSize = 0;
            char *filesContent;

            filesContent = get_file_string_cleaned(files[i], &fileSize);

            long numb_shingles = fileSize - K_SHINGLE + 1;
            char **shingles = (char **) malloc(numb_shingles * sizeof(char *));
            if(shingles==NULL)
                exit(1);
            shingle_extract_buf(filesContent, numb_shingles, shingles);
            
            double start_get_signatures = omp_get_wtime();
            long long unsigned *signatures= get_signatures(shingles, numb_shingles);
            double  end_get_signatures = omp_get_wtime();
            exectimes(end_get_signatures-start_get_signatures, GET_SIGNATURES, SET_TIME);
            
            minhashDocumenti[i] = signatures;

            for (int j = 0; j < numb_shingles; j++)
                free(shingles[j]);
            free(shingles);
            free(filesContent);
    }
    
    end = omp_get_wtime();
    exectimes(end-start, MAIN, SET_TIME);

    start = omp_get_wtime();
    find_similarity(numberOfFiles, files, minhashDocumenti);
    end = omp_get_wtime();
    exectimes(end-start, FIND_SIMILARITY, SET_TIME);

    //test
    exectimes(threads, NUMBER_OF_FUNCTIONS, EXPORT_LOG);
    check_coherence(minhashDocumenti, numberOfFiles);

    for (int i = 0; i < numberOfFiles; i++) {
        free(files[i]);
        free(minhashDocumenti[i]);
    }
    free(files);
    free(minhashDocumenti);
    return 0;
}

int cmpfunc (const void * a, const void * b) {
    return strcmp( *(const char**)a, *(const char**)b );
}
