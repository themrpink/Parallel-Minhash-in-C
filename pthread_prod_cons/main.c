#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <unistd.h> 
#include <time.h>
#include <omp.h>
#include <string.h>

#include "hash_FNV_1.h"
#include "shingle_extract.h"
#include "documents_getters.h"
#include "tokenizer.h"
#include "get_similarities.h"
#include "time_test.h"
#include "queue.h"
#include "prod_cons.h"
#include "minhash.h"

//gcc -Wall -fopenmp -o main main.c documents_getters.c get_similarities.c hash_FNV_1.c  tokenizer.c shingle_extract.c time_test.c queue.c prod_cons.c minhash.c

#define  EXITARGUMENTFAIL 20
#define  EXITNOFILEFOUND  30
 

int main(int argc, char *argv[]) {

    omp_set_nested(1);
    omp_set_dynamic(0);
    omp_set_max_active_levels(2);

    struct timespec begin, begin2, end, end2;
    clock_gettime(CLOCK_REALTIME, &begin);
    clock_gettime(CLOCK_REALTIME, &begin2);     //per misurare il tempo effettivo di computazione

    char *folderName = argv[1];
    int numberOfThreads = atoi(argv[2]);
    int minhashTreads = (numberOfThreads-1)/3;
    int getSignaturesThreads = numberOfThreads - minhashTreads - 1;
    int numberOfFiles = countNumberOfFiles(folderName);
    if (numberOfFiles == 0) {
        exit(EXITNOFILEFOUND);
    }

    if(numberOfFiles<numberOfThreads)
        numberOfThreads = numberOfFiles;
    printf("numero di file: %d\n", numberOfFiles);

    char **files = (char**)calloc(numberOfFiles, sizeof(char*));
    long long unsigned **minhashDocumenti = (long long unsigned **) malloc(numberOfFiles*sizeof (long long unsigned*));

    //alloca memoria per i vari threads
    args_list_dir *args_producer =                          (args_list_dir *) malloc(sizeof(args_list_dir) );
    args_minHash *args_first_consumer =                     (args_minHash *) malloc(minhashTreads*sizeof(args_minHash));
    args_getSignatures_consumer *args_second_consumer =     (args_getSignatures_consumer*) malloc(getSignaturesThreads*sizeof(args_getSignatures_consumer));
    Prod_Cons_Data *files_struct =                          (Prod_Cons_Data*)malloc(sizeof (Prod_Cons_Data));
    Prod_Cons_Data *getSignatures_struct =                  (Prod_Cons_Data*)malloc(sizeof (Prod_Cons_Data));

    //inizializza i mutex, le condition variable, le code, i thread
    initialize(files_struct);
    initialize(getSignatures_struct);
    files_struct->queue = createQueue(10);
    getSignatures_struct->queue = createQueue(10);
    pthread_t *thread_handles;
    thread_handles = (pthread_t *) malloc((numberOfThreads) * sizeof(pthread_t));

    //lancia i threads
    for(int i=0; i<numberOfThreads; i++){
        if(i<1){                                                       //il primo thread è per il produttore list_dir()
            args_producer->files_struct = files_struct;
            args_producer->numberOfFiles = numberOfFiles;
            args_producer->nomeDirectory = folderName;
            pthread_create(&thread_handles[i], NULL, list_dir, (void *) args_producer);
        }
        else if (i <= minhashTreads){                                              //questi thread sono per il produttore-consumatore minHash()
            args_first_consumer[i-1].files_struct = files_struct;
            args_first_consumer[i-1].files = files;
            args_first_consumer[i-1].rank = i-1;
            args_first_consumer[i-1].numberOfThreads = minhashTreads;
            args_first_consumer[i-1].numberOfFiles = numberOfFiles;
            args_first_consumer[i-1].minhashDocumenti = minhashDocumenti;
            args_first_consumer[i-1].signatures_struct = getSignatures_struct;
            pthread_create(&thread_handles[i], NULL, minHash, (void *) &args_first_consumer[i-1]);
        }
        else {                                                           //questi thread sono per il consumatore get_signatures()
            args_second_consumer[i-minhashTreads-1].rank = i-minhashTreads-1;
            args_second_consumer[i-minhashTreads-1].numberOfFiles = numberOfFiles / (numberOfThreads-minhashTreads-1);
            if(args_second_consumer[i-minhashTreads-1].rank== numberOfThreads-minhashTreads-1-1) 
                args_second_consumer[i-minhashTreads-1].numberOfFiles += numberOfFiles % (numberOfThreads-minhashTreads-1);
            args_second_consumer[i-minhashTreads-1].signatures_struct = getSignatures_struct;
            pthread_create(&thread_handles[i], NULL, get_signatures, (void *) &args_second_consumer[i-minhashTreads-1]);
        }
    }

    for(int i=0; i<numberOfThreads; i++){ 
        pthread_join(thread_handles[i], NULL);
    }

    exectimes(getElapsedTime(&begin, &end), MAIN, SET_TIME);

    clock_gettime(CLOCK_REALTIME, &begin);
    find_similarity(numberOfFiles, files, minhashDocumenti);
    exectimes(getElapsedTime(&begin, &end), FIND_SIMILARITY, SET_TIME);
    
    exectimes(getElapsedTime(&begin2, &end2), TOTAL_TIME, SET_TIME);
    free(files);
    exectimes(numberOfThreads, NUMBER_OF_FUNCTIONS, EXPORT_LOG);
    //check_coherence(minhashDocumenti, numberOfFiles);

    return 0;
}

