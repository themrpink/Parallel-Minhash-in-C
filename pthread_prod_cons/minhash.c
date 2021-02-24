#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h> 
#include <time.h>
#include "minhash.h"
#include "hash_FNV_1.h"
#include "shingle_extract.h"
#include "documents_getters.h"
#include "tokenizer.h"
#include "get_similarities.h"
#include "time_test.h"
#include "string.h"
#include "prod_cons.h"

/*
 * io lancio un producer che calcola numero di files, e nomi dei files e li mette in uno struct di risultati
 * ogni volta che c'é un risultato devo inviare il messaggio al consumer, che si attiva e lo usa
*/
void *minHash(void * args){

    //estrae gli argomenti
    args_minHash *argomenti = (args_minHash*) args;
    char *filename;

    //gestisce gli indici per ogni thread in base al rank
    int start_loop = (argomenti->numberOfFiles / argomenti->numberOfThreads) * (argomenti->rank);
    int end_loop = (argomenti->numberOfFiles / argomenti->numberOfThreads) * (1+argomenti->rank);
    if (1+argomenti->rank == argomenti->numberOfThreads)
        end_loop = argomenti->numberOfFiles;

    for (int i = start_loop; i < end_loop; ++i) {
          
        filename = (char*)consumer(argomenti->files_struct);
        argomenti->files[i] = strdup(filename);

        long fileSize = 0;
        char *filesContent;
        filesContent = get_file_string_cleaned(filename, &fileSize);

        long numb_shingles = fileSize - K_SHINGLE + 1;
        char **shingles = (char **) malloc(numb_shingles * sizeof(char *));
        shingle_extract_buf(filesContent, numb_shingles, shingles);

        struct getSignatures_producer_args *queue_args = init_getSignatures_args(numb_shingles, shingles, argomenti->minhashDocumenti+i);
        producer(argomenti->signatures_struct, (void*)queue_args);

        free(filesContent);                                                                       
    }
return 0;
}

struct getSignatures_producer_args* init_getSignatures_args(long numb_shingles, char **shingles, long long unsigned **minhashDocumenti){
    struct getSignatures_producer_args *queue_args = (struct getSignatures_producer_args *) malloc(sizeof(struct getSignatures_producer_args));
    queue_args->numb_shingles = numb_shingles;
    queue_args->shingles = shingles;
    queue_args->minhashDocumenti = minhashDocumenti; 
    return queue_args;
}