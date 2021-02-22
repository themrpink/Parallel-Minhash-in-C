//
// Created by stefano on 29/01/21.
//
#include <pthread.h>
#include "prod_cons.h"
#include "queue.h"
#include <stdio.h>


void initialize(Prod_Cons_Data *data) {

    pthread_mutex_init(&data->queue_mutex, NULL);
    pthread_cond_init(&data->empty, NULL);
    pthread_cond_init(&data->full, NULL);
}


void * producer (Prod_Cons_Data *data, void* item){

    pthread_mutex_lock(&data->queue_mutex);                     // acquisisce il  lock
    
    while (isFull(data->queue))                                // controlla se la coda è piena
        pthread_cond_wait(&data->empty, &data->queue_mutex);    // aspetta che si liberi spazio nella coda
    enqueue(data->queue, item);                                 //inserisce l`elemento nella coda (produce)
    pthread_cond_signal(&data->full);                           // segnala che un elemento è stato inserito nella coda
    pthread_mutex_unlock(&data->queue_mutex);                   // rilascia il lock
}

void * consumer (Prod_Cons_Data *data){

    pthread_mutex_lock(&data->queue_mutex);                     // acquisisce il lock

    while (isEmpty(data->queue))                               // controlla se la coda è vuota
        pthread_cond_wait(&data->full, &data->queue_mutex);    // aspetta che ci sia un elemento nella coda 
    void *tmp = dequeue(data->queue);                           // consuma l'elemento rimuovendolo dalla coda
    
    pthread_cond_signal(&data->empty);                          // segnala che un elemento è stato rimosso
    pthread_mutex_unlock(&data->queue_mutex);                   // rilascia il lock
    return tmp;
}

