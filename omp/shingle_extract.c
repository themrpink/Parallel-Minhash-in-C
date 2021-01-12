
#include "shingle_extract.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "time_test.h"

void shingle_extract_buf(char* buf, long numb_shingles, char **shingles){

    double start;
    double end;

    start=omp_get_wtime();

    //li estrae e salva in **shingles
    #pragma omp parallel for collapse(2)
        for (long count = 0; count < numb_shingles; count++) {
            for (int pos = 0; pos < K_SHINGLE; pos++)
                shingles[count][pos] = buf[count + pos];
        }

    end = omp_get_wtime();
    exectimes(end-start, SHINGLE_EXTRACT, SET_TIME);

}

