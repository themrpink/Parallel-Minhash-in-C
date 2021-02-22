//
// Created by stefano on 29/01/21.
//

#ifndef OMP_PROD_CONS_H
#define OMP_PROD_CONS_H

#include "queue.h"
#include <semaphore.h> 
#include <unistd.h> 
#define STOP    1

/*
    struttura che rappresenta un'interfaccia produttore-consumatore
*/
 typedef struct Prod_Cons_Data {
    pthread_cond_t empty;
    pthread_cond_t full;
    pthread_mutex_t queue_mutex;
    struct Queue *queue;
}Prod_Cons_Data;

/*
    inizializza il mutex e le condition variables di un produttore-consumatore
*/
void initialize(Prod_Cons_Data *data);

/*
    produttore: chiama un mutex, inserisce elemento nella coda appena c'è spazio disponibile, rilascia un eventuale thread consumatore in attesa
    dell'elemento inserito nella coda, rilascia il mutex
*/
void * producer (Prod_Cons_Data *data, void* item);

/*
    consuma gli elementi nella coda
*/
void * consumer (Prod_Cons_Data *data);

/*
    funzione di minHash
*/
void *minHash(void * args);




/*
    gli argomenti necessari alla funzione list_dir. Questa funziona opera da produttore sulla
    coda dei file da elaborare.
*/
typedef struct args_list_dir {
    char ***files;
    char *nomeDirectory;
    int numberOfFiles;
    Prod_Cons_Data *files_struct;
}args_list_dir;

/*
    gli argomenti necessari alla funzione minHash.
    Questa funziona sia da consumatore sulla coda dei file, che da produttore di shingles da ridurre in signatures, usando
    un'altra apposita coda.
*/
typedef struct args_minHash {
    char *files;
    int numberOfFiles;
    int rank;
    int numberOfThreads;
    long long unsigned **minhashDocumenti;
    Prod_Cons_Data *files_struct;
    Prod_Cons_Data *signatures_struct;
}args_minHash ;

/*
    argomenti della funzione getSignatures(). Questa consuma dalla coda degli shingles e restituisce le signatures di ogni file.
*/
typedef struct args_getSignatures {
    int rank;
    int numberOfFiles;
    sem_t *mutex; 
    Prod_Cons_Data *signatures_struct;
}args_getSignatures;


/*
    inizializza gli agormenti di getSignatures()
*/
struct getSignatures_args* init_getSignatures_args(long numb_shingles, char **shingles, long long unsigned **minhashDocumenti);


#endif //OMP_PROD_CONS_H
