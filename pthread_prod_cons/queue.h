//
// Created by stefano on 30/01/21.
//

#ifndef OMP_QUEUE_H
#define OMP_QUEUE_H

// struttura che rappresenta una coda
struct Queue {
    int front, rear, size;
    unsigned capacity;
    void** array;
};

/*
    crea una coda, allocandogli lo spazio necessario
*/
struct Queue* createQueue(unsigned capacity);

/*
    controlla se la coda è piena
*/
int isFull(struct Queue* queue);

/*
    controlla se la coda è vuota
*/
int isEmpty(struct Queue* queue);

/*
    inserisce un elemento nella coda
*/
int enqueue(struct Queue* queue, void *item);

/*
    elimina un elemento dalla coda
*/
void* dequeue(struct Queue* queue);


#endif //OMP_QUEUE_H
