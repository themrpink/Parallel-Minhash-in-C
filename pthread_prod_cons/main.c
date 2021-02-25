#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h> 
#include <unistd.h> 
#include <time.h>
#include "hash_FNV_1.h"
#include "shingle_extract.h"
#include "documents_getters.h"
#include "tokenizer.h"
#include "get_similarities.h"
#include "time_test.h"
#include <string.h>
#include "queue.h"
#include "prod_cons.h"
#include "minhash.h"

//gcc -Wall -fopenmp -o main main.c documents_getters.c get_similarities.c hash_FNV_1.c  tokenizer.c shingle_extract.c time_test.c queue.c prod_cons.c minhash.c

#define  EXITARGUMENTFAIL 20
#define  EXITNOFILEFOUND  30
#define COEFFICIENTE_SIMILARITA 0.75



sem_t mutex; 

int main(int argc, char *argv[]) {

    char *folderName = argv[1];
    int numberOfThreads  = atoi(argv[2]);

    int numberOfFiles = countNumberOfFiles(folderName);
    if (numberOfFiles == 0) {
        exit(EXITNOFILEFOUND);
    }

    if(numberOfFiles<numberOfThreads)
        numberOfThreads = numberOfFiles;
    printf("numero di file: %d\n", numberOfFiles);
    char **files = (char**)calloc(numberOfFiles, sizeof(char*));
    //per misurare il tempo effettivo di computazione
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);


    long long unsigned **minhashDocumenti = (long long unsigned **) malloc(numberOfFiles*sizeof (long long unsigned*));

    //alloca memoria per i vari threads
    args_list_dir *args_producer =                          (args_list_dir *) malloc(sizeof(args_list_dir) );
    args_minHash *args_first_consumer =                     (args_minHash *) malloc(numberOfThreads*sizeof(args_minHash));
    args_getSignatures_consumer *args_second_consumer =     (args_getSignatures_consumer*) malloc(numberOfThreads*sizeof(args_getSignatures_consumer));
    Prod_Cons_Data *files_struct =                          (Prod_Cons_Data*)malloc(sizeof (Prod_Cons_Data));
    Prod_Cons_Data *getSignatures_struct =                  (Prod_Cons_Data*)malloc(sizeof (Prod_Cons_Data));

    //inizializza i mutex, le code e il semaforo
    initialize(files_struct);
    initialize(getSignatures_struct);
    files_struct->queue = createQueue(10);
    getSignatures_struct->queue = createQueue(10);
    sem_init(&mutex, 0, 1);             //semaforo utilizzato in get_signatures(), inizializzato a 1 e abilitato per i thread

    pthread_t *thread_handles;
    thread_handles = (pthread_t *) malloc((2*numberOfThreads+1) * sizeof(pthread_t));

    //lancia i threads
    for(int i=0; i<2*numberOfThreads+1; i++){
        if(i==0){                                                       //il primo thread è per il produttore list_dir()
            args_producer->files_struct = files_struct;
            args_producer->numberOfFiles = numberOfFiles;
            args_producer->nomeDirectory = folderName;
            pthread_create(&thread_handles[i], NULL, list_dir, (void *) args_producer);
        }
            
        else if (i <= numberOfThreads){                                 //questi thread sono per il produttore-consumatore minHash()
            args_first_consumer[i-1].files_struct = files_struct;
            args_first_consumer[i-1].files = files;
            args_first_consumer[i-1].rank = i-1;
            args_first_consumer[i-1].numberOfThreads = numberOfThreads;
            args_first_consumer[i-1].numberOfFiles = numberOfFiles;
            args_first_consumer[i-1].minhashDocumenti = minhashDocumenti;
            args_first_consumer[i-1].signatures_struct = getSignatures_struct;
            pthread_create(&thread_handles[i], NULL, minHash, (void *) &args_first_consumer[i-1]);
        }
        else{                                                           //questi thread sono per il consumatore get_signatures()
            args_second_consumer[i-numberOfThreads-1].rank = i-numberOfThreads-1;
            args_second_consumer[i-numberOfThreads-1].numberOfFiles = numberOfFiles;
            args_second_consumer[i-numberOfThreads-1].mutex = &mutex;
            args_second_consumer[i-numberOfThreads-1].signatures_struct = getSignatures_struct;
            pthread_create(&thread_handles[i], NULL, get_signatures, (void *) &args_second_consumer[i-numberOfThreads-1]);
        }
    }


    for(int i=0; i<2*numberOfThreads+1; i++){ 
        pthread_join(thread_handles[i], NULL);
    }

    sem_destroy(&mutex); 
    exectimes(getElapsedTime(&begin, &end), MAIN, SET_TIME);

    clock_gettime(CLOCK_REALTIME, &begin);
    find_similarity(numberOfFiles, files, minhashDocumenti);
    exectimes(getElapsedTime(&begin, &end), FIND_SIMILARITY, SET_TIME);

    free(files);
    exectimes(numberOfThreads, NUMBER_OF_FUNCTIONS, EXPORT_LOG);
    //check_coherence(minhashDocumenti, numberOfFiles);

    return 0;
}

