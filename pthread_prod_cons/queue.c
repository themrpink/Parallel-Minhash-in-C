//
// Created by stefano on 30/01/21.
//

#include "queue.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>




struct Queue* createQueue(unsigned capacity){

    struct Queue* queue = (struct Queue*)malloc(
            sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    queue->rear = capacity - 1;
    queue->array = (void**)malloc(
            queue->capacity * sizeof(void*));
    return queue;
}


int isFull(struct Queue* queue){

    return (queue->size == queue->capacity);
}

 
int isEmpty(struct Queue* queue){

    return (queue->size == 0);
}


int enqueue(struct Queue* queue, void *item){
    
    if (isFull(queue))
        return -1;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    return 0;
}


void* dequeue(struct Queue* queue){
    
    if (isEmpty(queue))
        return NULL;
    void * item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}


