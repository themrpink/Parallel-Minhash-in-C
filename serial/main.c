//
// Created by dani2612 on 19/12/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "main.h"
#include <dirent.h>
#include "hash_FNV_1.c"
#include "shingle_extract.c"
#include "documents_getters.c"
#include "tokenizer.c"
#include "get_similarities.c"






//folder e ricerca dei file
int main(int argc, char *argv[]) {
    if (argc != 1) {
        exit(EXITARGUMENTFAIL);
    }
    char *folderName = argv[1];
    char **files;
    int numberOfFiles = 0;
    int fileNameLength = list_dir(folderName, files, &numberOfFiles);
    long long unsigned *minhashDocumenti[numberOfFiles];


    struct sign_doc **files_sketches;
    files_sketches = (struct sign_doc**) malloc(numberOfFiles * sizeof(struct sign_doc*));

    for(int i=0; i<numberOfFiles; i++)
        files_sketches[i]=(struct sign_doc*)malloc(N_SIGNATURES*sizeof(struct sign_doc));


    for (int i = 0; i < numberOfFiles; ++i) {
        long fileSize = 0;  
        char *filesContent = get_file_string_cleaned(files[i], &fileSize);

        long numb_shingles = fileSize - K_SHINGLE +1;
        char **shingles = (char**) malloc(numb_shingles * sizeof(char*));
        shingle_extract_buf(filesContent,numb_shingles,shingles);

        long long unsigned *signatures= get_signatures(shingles, numb_shingles);
        minhashDocumenti[i] = signatures;

        get_sketches(files_sketches[i], signatures, files[i]);
        for(long long i=0; i<numb_shingles; i++)
            free(shingles[i]);
        free(shingles);
    }

    find_similarity(numberOfFiles, files_sketches);


    for (int i = 0; i < numberOfFiles; ++i) {
        for (int j = 0; j < numberOfFiles; ++j) {
            InfoFile infoCoppia;
            long totalSignaturesEquals = 0;
            for (int k = 0; k < N_SIGNATURES; ++k) {
                if (minhashDocumenti[i][k] == minhashDocumenti[j][k]) {
                    totalSignaturesEquals++;
                }
            }
            double coefficient = totalSignaturesEquals / (N_SIGNATURES*2);
            printf("File %s e %s sono simili %s",files[i],files[j],coefficient > COEFFICIENTE_SIMILARITA ? "Si" : "No");
//            strcpy(infoCoppia.filePrimo, files[i]);
//            strcpy(infoCoppia.fileSecondo, files[i]);
//            infoCoppia.simili = coefficient > COEFFICIENTE_SIMILARITA ? 1 : 0;
        }
    }
    return 0;
}

