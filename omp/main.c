#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "hash_FNV_1.h"
#include "shingle_extract.h"
#include "documents_getters.h"
#include "tokenizer.h"
#include "get_similarities.h"

#define  EXITARGUMENTFAIL 20
#define  EXITNOFILEFOUND  30
#define COEFFICIENTE_SIMILARITA 0.75

//folder e ricerca dei fileparallel
int main(int argc, char *argv[]) {

    omp_set_num_threads(9);
    char *folderName = argv[1];
    char **files;
    int numberOfFiles = list_dir(folderName, &files);
    if (numberOfFiles==0){
        exit(EXITNOFILEFOUND);
    }

    long long unsigned *minhashDocumenti[numberOfFiles];
    #pragma omp parallel for
    for (int i = 0; i < numberOfFiles; ++i) {
        long fileSize=0;
        char *filesContent = get_file_string_cleaned(files[i], &fileSize);
        long numb_shingles = fileSize - K_SHINGLE +1;
        char **shingles = (char**) malloc(numb_shingles * sizeof(char*));
        shingle_extract_buf(filesContent,numb_shingles,shingles);
        long long unsigned *signatures= get_signatures(shingles, numb_shingles,3);
        minhashDocumenti[i] = signatures;

        free(shingles);
        free(filesContent);
    }

    struct doc_couple* couples;
    int num_of_doc_couples = find_similarity(numberOfFiles, files, minhashDocumenti);

    free(files);
    //find_similarity restituisce un vettore "*couples" con tutte le coppie di documenti che condividono almeno una signature.
    //queste sono coppie di id di documenti, ma l'id è un intero uguale alla posizione del file in minhashDocumenti.
    //quindi per confrontare le signatures basta scorrere il vettore couples, estrarre le id e con queste estrarre le signatures relative
    //a entrambi i documenti in minhashDocumenti e confrontarle.


    return 0;
}

