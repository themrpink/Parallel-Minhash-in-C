//
// Created by dani2612 on 19/12/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "main.h"
#include <dirent.h>
#include "shingle_extract.h"
#include "documents_getters.h"
#include "tokenizer.h"
#include "hash_FNV_1.h"




//folder e ricerca dei file
int main(int argc, char *argv[]) {
    if (argc != 1) {
        exit(EXITARGUMENTFAIL);
    }
    char *folderName = argv[1];
    char **files;
    int numberOfFiles = 0;
    int fileNameLength = list_dir(folderName, &files, &numberOfFiles);

    long minhashDocumenti[numberOfFiles][PRIMES_SIZE];
    for (int i = 0; i < numberOfFiles; ++i) {
        long fileSize = 0;
        char **shingles;
        long shinglesLength = 0;
        char *filesContent = get_file_string_cleaned(files[i], &fileSize);
//      shingle_extract_buf(filesContent,fileSize,shingles,&shinglesLength);
        for (int j = 0; j < PRIMES_SIZE; ++j) {
            u_int64_t minhashDocumento = get_signatures(shingles, shinglesLength);
            minhashDocumenti[i][j] = minhashDocumento;
        }
    }

    for (int i = 0; i < numberOfFiles; ++i) {
        for (int j = 0; j < numberOfFiles; ++j) {
            InfoFile infoCoppia;
            long totalSignaturesEquals = 0;
            for (int k = 0; k < PRIMES_SIZE; ++k) {
                if (minhashDocumenti[i][k] == minhashDocumenti[j][k]) {
                    totalSignaturesEquals++;
                }
            }
            double coefficient = totalSignaturesEquals / PRIMES_SIZE;
            printf("File %s e %s sono simili %s",files[i],files[j],coefficient > COEFFICIENTE_SIMILARITA ? "Si" : "No");
//            strcpy(infoCoppia.filePrimo, files[i]);
//            strcpy(infoCoppia.fileSecondo, files[i]);
//            infoCoppia.simili = coefficient > COEFFICIENTE_SIMILARITA ? 1 : 0;
        }
    }

}

