#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hash_FNV_1.h"
#include "shingle_extract.h"
#include "documents_getters.h"
#include "tokenizer.h"
#include "get_similarities.h"
#include "time_test.h"
#include "string.h"
#include <pthread.h>
#include <time.h>

#define  EXITARGUMENTFAIL 20
#define  EXITNOFILEFOUND  30
#define COEFFICIENTE_SIMILARITA 0.75
#define NUMB_THREADS 2

//gcc -Wall -fopenmp -lpthread -o main main.c documents_getters.c get_similarities.c hash_FNV_1.c  tokenizer.c shingle_extract.c time_test.c
int cmpfunc (const void * a, const void * b);

typedef struct MinHashParameters{
    char **files;
    int numberOfFiles;
    int numberOfThreads;
    int rank;
    long long unsigned **minhashDocumenti;
}MinHashParameters;

void *minHash(void* args);

int main(int argc, char *argv[]) {

    char *folderName = argv[1];
    int thread_count = atoi(argv[2]);
    char **files;

    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    int numberOfFiles = list_dir(folderName, &files);
    if (numberOfFiles==0){
        exit(EXITNOFILEFOUND);
    }
    printf("numero di file: %d\n", numberOfFiles);
    exectimes(getElapsedTime(&begin, &end), LIST_DIR, SET_TIME);

    //ordina i nomi dei file giusto per far funzionare il test sulle signatures
    qsort(files, numberOfFiles, sizeof(files[0]),cmpfunc  );
    long long unsigned **minhashDocumenti = (long long unsigned **) malloc(numberOfFiles*sizeof (long long unsigned*));
    
    clock_gettime(CLOCK_REALTIME, &begin);
    
    if(thread_count>numberOfFiles)
        thread_count = numberOfFiles;
    pthread_t *thread_handles;
    thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));

    MinHashParameters argomenti[thread_count];
    for (int i = 0; i < thread_count; ++i) {
        argomenti[i].files = files;
        argomenti[i].numberOfFiles = numberOfFiles;
        argomenti[i].numberOfThreads = thread_count;
        argomenti[i].rank = i;
        argomenti[i].minhashDocumenti = minhashDocumenti;
    }

    int thread;
    for (thread=0; thread< thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, minHash, (void*) &argomenti[thread]);

    for ( thread = 0; thread<thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);


    exectimes(getElapsedTime(&begin, &end), MAIN, SET_TIME);
    
    clock_gettime(CLOCK_REALTIME, &begin);
    find_similarity(numberOfFiles, files, minhashDocumenti);
    exectimes(getElapsedTime(&begin, &end), FIND_SIMILARITY, SET_TIME);


    free(files);

    //test
    exectimes(NUMB_THREADS, NUMBER_OF_FUNCTIONS, EXPORT_LOG);
    //check_coherence(minhashDocumenti, numberOfFiles);

    return 0;
}

void* minHash( void *args){

    MinHashParameters *argomenti = (MinHashParameters*) args;
    struct timespec begin, end;
    int start_loop = (argomenti->numberOfFiles / argomenti->numberOfThreads) * argomenti->rank;
    int end_loop = (argomenti->numberOfFiles / argomenti->numberOfThreads) * (1+argomenti->rank);
    if (1+argomenti->rank == argomenti->numberOfThreads-1)
        end_loop = argomenti->numberOfFiles;

    for (int i = start_loop; i < end_loop; ++i) {

        long fileSize = 0;
        char *filesContent;

        clock_gettime(CLOCK_REALTIME, &begin);
        filesContent = get_file_string_cleaned(argomenti->files[i], &fileSize);
        exectimes(getElapsedTime(&begin, &end), GET_FILE_STRINGS_CLEANED, SET_TIME);

        long numb_shingles = fileSize - K_SHINGLE + 1;
        char **shingles = (char **) malloc(numb_shingles * sizeof(char *));
        
        clock_gettime(CLOCK_REALTIME, &begin);
        shingle_extract_buf(filesContent, numb_shingles, shingles);
        exectimes(getElapsedTime(&begin, &end), SHINGLE_EXTRACT, SET_TIME);

        clock_gettime(CLOCK_REALTIME, &begin);
        long long unsigned *signatures = get_signatures(shingles, numb_shingles);
        exectimes(getElapsedTime(&begin, &end), GET_SIGNATURES, SET_TIME);

        argomenti->minhashDocumenti[i] = signatures;
        for (int j = 0; j < numb_shingles; j++)
            free(shingles[j]);
        free(shingles);
        free(filesContent);

    }
    return 0;
}

int cmpfunc (const void * a, const void * b) {
    return strcmp( *(const char**)a, *(const char**)b );
}
