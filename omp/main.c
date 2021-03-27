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

//gcc -Wall -fopenmp -o main main.c documents_getters.c get_similarities.c hash_FNV_1.c  tokenizer.c shingle_extract.c time_test.c
//sudo perf stat -e L1-dcache-load-misses ./omp "../docs/docs_medium" 4 4
//./main "/home/stefano/Scrivania/Collegamento a uni/terzo anno/multicore 2020/progetto/git/minhash-multicore/docs" 16

int global_thread_numb = 2;

int main(int argc, char *argv[]) {

    int threads=atoi(argv[2]);
    global_thread_numb=atoi(argv[3]);
    printf("threads: %d\n", threads);
    omp_set_num_threads(threads);
    omp_set_nested(1);
    omp_set_dynamic(0);
    omp_set_max_active_levels(3);

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

    long long unsigned **minhashDocumenti = (long long unsigned **)malloc(numberOfFiles*sizeof (long long unsigned *));

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < numberOfFiles; i++) {

            long fileSize = 0;
            char *filesContent;

            filesContent = get_file_string_cleaned(files[i], &fileSize);

            long numb_shingles = fileSize - K_SHINGLE + 1;
            char **shingles = (char **) malloc(numb_shingles * sizeof(char *));
            if(shingles==NULL)
                exit(1);
            shingle_extract_buf(filesContent, numb_shingles, shingles);
            
            long long unsigned *signatures= get_signatures(shingles, numb_shingles);
            
            minhashDocumenti[i] = signatures;

            for (int j = 0; j < numb_shingles; j++)
                free(shingles[j]);
            free(shingles);
            free(filesContent);
    }
    
    end = omp_get_wtime();
    exectimes(end-start, MAIN, SET_TIME);
    
    double start2 = omp_get_wtime();
    find_similarity(numberOfFiles, files, minhashDocumenti);
    end = omp_get_wtime();
    exectimes(end-start2, FIND_SIMILARITY, SET_TIME);
    exectimes(end-start, TOTAL_TIME, SET_TIME);
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


