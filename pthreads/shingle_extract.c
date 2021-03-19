#include "shingle_extract.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "time_test.h"
#include <pthread.h>
#include "omp.h"

int thread_count=4;

typedef struct {
    long rank;
    char* buf;
    char** shingles;
    long numb_shingles;
} Create_shingles_args;

void shingle_extract_buf(char* buf, long numb_shingles, char **shingles){
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    //void(*create_shingles_ptr)(void*)=create_shingles;
    pthread_t* thread_handles=malloc(thread_count*sizeof (pthread_t));
    Create_shingles_args argomenti[thread_count];
    for (int i = 0; i < thread_count; ++i) {
        argomenti[i].buf=buf;
        argomenti[i].shingles=shingles;
        argomenti[i].numb_shingles=numb_shingles;
        argomenti[i].rank=(long)i;
    }

    int j=0;
    while(j<thread_count){
       int thread_created=pthread_create(&thread_handles[j],NULL,create_shingles,(void*)&argomenti[j]);
       if (thread_created==0){
            j++;
       }
    }

    for (int i = 0; i < thread_count; ++i) {
        pthread_join(thread_handles[i],NULL);
    }

    shingles=argomenti[0].shingles;
    free(thread_handles);
    exectimes(getElapsedTime(&begin, &end), SHINGLE_EXTRACT, SET_TIME);
}

void *create_shingles(void* args){
    long numThread=((Create_shingles_args*)args)->rank;
    long count;
    int local_numb_shingles=((Create_shingles_args*)args)->numb_shingles/thread_count;
    long firstRow=numThread*local_numb_shingles;
    long lastRow;
    if ((numThread+1)==thread_count){
         lastRow=((Create_shingles_args*)args)->numb_shingles-1;
    }else{
         lastRow=(numThread+1)*local_numb_shingles-1;
    }



    for(count=firstRow; count <= lastRow; count++) {
       ((Create_shingles_args*)args)->shingles[count] = (char *)malloc(K_SHINGLE*(sizeof(char)));
        for (int pos = 0; pos < K_SHINGLE; pos++){
            ((Create_shingles_args*)args)->shingles[count][pos] =((Create_shingles_args*)args)->buf[count + pos];
        }

    }
}
