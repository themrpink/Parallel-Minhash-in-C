#include "shingle_extract.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "time_test.h"
#include <omp.h>

void shingle_extract_buf(char* buf, long numb_shingles, char **shingles){

    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);


    #pragma omp parallel for num_threads(2) 
        for(int i=0; i<numb_shingles; i++)
            shingles[i] = (char *)malloc(K_SHINGLE*(sizeof(char)));

    
    #pragma omp parallel for collapse(2) num_threads(2) 
        for (int count = 0; count < numb_shingles; count++) {
            for (int pos = 0; pos < K_SHINGLE; pos++){
                shingles[count][pos] = buf[count + pos];

            }
        }
/*
    for(long long i=0; i<numb_shingles; i++)
        shingles[i] = (char *)malloc(K_SHINGLE*(sizeof(char)));

    //li estrae e salva in **shingles
    for(int count=0; count<numb_shingles; count++){
        for(int pos=0; pos<K_SHINGLE; pos++)
            shingles[count][pos]=buf[count+pos];   
    }
*/
    exectimes(getElapsedTime(&begin, &end), SHINGLE_EXTRACT, SET_TIME);
}
