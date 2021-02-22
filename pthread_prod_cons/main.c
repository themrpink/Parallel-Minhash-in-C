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
#include "string.h"
#include "queue.h"
#include "prod_cons.h"


#define  EXITARGUMENTFAIL 20
#define  EXITNOFILEFOUND  30
#define COEFFICIENTE_SIMILARITA 0.75
#define THREADS 4


sem_t mutex; 

int main(int argc, char *argv[]) {

    char *folderName = argv[1];
    char **files;
    int numberOfFiles = countNumberOfFiles(folderName);
    if (numberOfFiles == 0) {
        exit(EXITNOFILEFOUND);
    }
    
    //per misurare il tempo effettivo di computazione
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);


    long long unsigned **minhashDocumenti = (long long unsigned **) malloc(numberOfFiles*sizeof (long long unsigned*));

    //alloca memoria per i vari threads
    args_list_dir *args_producer =              (args_list_dir *) malloc(sizeof(args_list_dir) );
    args_minHash *args_first_consumer =         (args_minHash *) malloc(THREADS*sizeof(args_minHash));
    args_getSignatures *args_second_consumer =  (args_getSignatures*) malloc(THREADS*sizeof(args_getSignatures));
    Prod_Cons_Data *files_struct =              (Prod_Cons_Data*)malloc(sizeof (Prod_Cons_Data));
    Prod_Cons_Data *getSignatures_struct =      (Prod_Cons_Data*)malloc(sizeof (Prod_Cons_Data));

    //inizializza i mutex, le code e il semaforo
    initialize(files_struct);
    initialize(getSignatures_struct);
    files_struct->queue = createQueue(10);
    getSignatures_struct->queue = createQueue(10);
    sem_init(&mutex, 0, 1);             //semaforo utilizzato in get_signatures(), inizializzato a 1 e abilitato per i thread

    pthread_t *thread_handles;
    thread_handles = (pthread_t *) malloc((2*THREADS+1) * sizeof(pthread_t));

    //lancia i threads
    for(int i=0; i<2*THREADS+1; i++){
        if(i==0){                                               //il primo thread è per il produttore list_dir()
            args_producer->files_struct = files_struct;
            args_producer->numberOfFiles = numberOfFiles;
            args_producer->nomeDirectory = folderName;
            args_producer->files = &files;
            pthread_create(&thread_handles[i], NULL, list_dir, (void *) args_producer);
        }
            
        else if (i <= THREADS){                                 //questi thread sono per il produttore-consumatore minHash()
            args_first_consumer[i-1].files_struct = files_struct;
            args_first_consumer[i-1].rank = i-1;
            args_first_consumer[i-1].numberOfThreads = THREADS;
            args_first_consumer[i-1].numberOfFiles = numberOfFiles;
            args_first_consumer[i-1].minhashDocumenti = minhashDocumenti;
            args_first_consumer[i-1].signatures_struct = getSignatures_struct;
            pthread_create(&thread_handles[i], NULL, minHash, (void *) &args_first_consumer[i-1]);
        }
        else{                                                   //questi thread sono per il consumatore get_signatures()
            args_second_consumer[i-THREADS-1].rank = i-THREADS-1;
            args_second_consumer[i-THREADS-1].numberOfFiles = numberOfFiles;
            args_second_consumer[i-THREADS-1].mutex = &mutex;
            args_second_consumer[i-THREADS-1].signatures_struct = getSignatures_struct;
            pthread_create(&thread_handles[i], NULL, get_signatures, (void *) &args_second_consumer[i-THREADS-1]);
        }
    }


    for(int i=0; i<2*THREADS+1; i++){ 
        pthread_join(thread_handles[i], NULL);
    }
    sem_destroy(&mutex); 
    exectimes(getElapsedTime(&begin, &end), MAIN, SET_TIME);

    clock_gettime(CLOCK_REALTIME, &begin);
    find_similarity(numberOfFiles, files, minhashDocumenti);
    exectimes(getElapsedTime(&begin, &end), FIND_SIMILARITY, SET_TIME);


    free(files);

    exectimes(THREADS, NUMBER_OF_FUNCTIONS, EXPORT_LOG);
    check_coherence(minhashDocumenti, numberOfFiles);

    return 0;
}


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
    
        long fileSize = 0;
        char *filesContent;
        filesContent = get_file_string_cleaned(filename, &fileSize);

        long numb_shingles = fileSize - K_SHINGLE + 1;
        char **shingles = (char **) malloc(numb_shingles * sizeof(char *));
        shingle_extract_buf(filesContent, numb_shingles, shingles);

        struct getSignatures_args *queue_args = init_getSignatures_args(numb_shingles, shingles, argomenti->minhashDocumenti+i);
        producer(argomenti->signatures_struct, (void*)queue_args);

        free(filesContent);                                                                       
    }
return 0;
}

struct getSignatures_args* init_getSignatures_args(long numb_shingles, char **shingles, long long unsigned **minhashDocumenti){
    struct getSignatures_args *queue_args = (struct getSignatures_args *) malloc(sizeof(struct getSignatures_args));
    queue_args->numb_shingles = numb_shingles;
    queue_args->shingles = shingles;
    queue_args->minhashDocumenti = minhashDocumenti; 
    return queue_args;
}

