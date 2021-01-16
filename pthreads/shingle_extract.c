#include "shingle_extract.h"
#include <stdlib.h>
#include <stdio.h>
#include "time_test.h"
#include <pthread.h>

int thread_count=10;

typedef struct {
    long rank;
    char* buf;
    char** shingles;
    long numb_shingles;
} Create_shingles_args;


void shingle_extract_buf(char* buf, long numb_shingles, char **shingles){
    double start;
    double end;

    void(*create_shingles_ptr)(void*)=create_shingles;
    pthread_t* thread_handles=malloc(thread_count*sizeof (pthread_t));
    Create_shingles_args *argomenti=(Create_shingles_args*)malloc(sizeof(Create_shingles_args));
    argomenti->buf=buf;
    argomenti->shingles=shingles;
    argomenti->numb_shingles=numb_shingles;


    for (int i = 0; i < thread_count; ++i) {
        argomenti->rank=i;
        pthread_create(&thread_handles[i],NULL,create_shingles_ptr,(void*)argomenti);
    }

    for (int i = 0; i < thread_count; ++i) {
        pthread_join(thread_handles[i],NULL);
    }
    shingles=argomenti->shingles;
    free(argomenti);
    exectimes(end-start, SHINGLE_EXTRACT, SET_TIME);

}

void create_shingles(void* args){
    long numThread=((Create_shingles_args*)args)->rank;
    long count;
    int local_numb_shingles=((Create_shingles_args*)args)->numb_shingles/thread_count;
    long firstRow=numThread*local_numb_shingles;
    long lastRow=(numThread+1)*local_numb_shingles-1;


    for(count=firstRow; count <= lastRow; count++) {
       ((Create_shingles_args*)args)->shingles[count] = (char *)malloc(K_SHINGLE*(sizeof(char)));
        for (int pos = 0; pos < K_SHINGLE; pos++)
            ((Create_shingles_args*)args)->shingles[count][pos] =((Create_shingles_args*)args)->buf[count + pos];
    }
}



