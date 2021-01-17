
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "time_test.h"

#include "hash_FNV_1.h"
#include "shingle_extract.h"
#include "documents_getters.h"
#include "tokenizer.h"
#include "get_similarities.h"

#define  EXITARGUMENTFAIL 20
#define  EXITNOFILEFOUND  30
#define COEFFICIENTE_SIMILARITA 0.75

//folder e ricerca dei file
int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit(EXITARGUMENTFAIL);
    }

    char *folderName = argv[1];
    char **files;
    int numberOfFiles = list_dir(folderName, &files);
    if (numberOfFiles==0){
        exit(EXITNOFILEFOUND);
    }

    long long unsigned *minhashDocumenti[numberOfFiles];
    double start;
    double  end;

    start = omp_get_wtime();
    for (int i = 0; i < numberOfFiles; ++i) {
        long fileSize=0;
        char *filesContent = get_file_string_cleaned(files[i], &fileSize);
        long numb_shingles = fileSize - K_SHINGLE +1;
        char **shingles = (char**) malloc(numb_shingles * sizeof(char*));
        shingle_extract_buf(filesContent,numb_shingles,shingles);

        long long unsigned *signatures= get_signatures(shingles, numb_shingles);
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
    
    exectimes(1, NUMBER_OF_FUNCTIONS, EXPORT_LOG);
    return 0;
}

