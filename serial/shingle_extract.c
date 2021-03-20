#include "shingle_extract.h"
#include <stdlib.h>
#include <omp.h>
#include "time_test.h"

/**
 * Dato il contenuto di un documento ne estrae gli shingles.
 * @param buf
 * @param numb_shingles
 * @param shingles
 */
void shingle_extract_buf(char* buf, long numb_shingles, char **shingles){
    double start;
    double end;

    start=omp_get_wtime();
    for(long long i=0; i<numb_shingles; i++)
        shingles[i] = (char *)malloc(K_SHINGLE*(sizeof(char)));

    //li estrae e salva in **shingles
    for(int count=0; count<numb_shingles; count++){
        for(int pos=0; pos<K_SHINGLE; pos++)
            shingles[count][pos]=buf[count+pos];   
    }
    end = omp_get_wtime();
    exectimes(end-start, SHINGLE_EXTRACT, SET_TIME);
}

