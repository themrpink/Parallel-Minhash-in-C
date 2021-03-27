#include "time_test.h"
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <time.h>

void exectimes(double value, enum Function_name function_name, enum Task task){

    static double time[NUMBER_OF_FUNCTIONS] = {0.0};

    if(task == SET_TIME) {
        switch (function_name) {
            case MAIN:
                if(value>time[MAIN])
                time[MAIN] = value;
                break;
            case LIST_DIR:
                if(value>time[LIST_DIR])
                time[LIST_DIR] = value;
                break;
            case COUNT_NUMBER_OF_FILES:
                if(value>time[COUNT_NUMBER_OF_FILES])
                    time[COUNT_NUMBER_OF_FILES] = value;
                break;
            case GET_FILE_STRINGS_CLEANED:
                if(value>time[GET_FILE_STRINGS_CLEANED])
                time[GET_FILE_STRINGS_CLEANED] = value;
                break;
            case COMPRESS_SPACES:
                if(value>time[COMPRESS_SPACES])
                time[COMPRESS_SPACES] = value;
                break;
            case SHINGLE_EXTRACT:
                if(value>time[SHINGLE_EXTRACT])
                time[SHINGLE_EXTRACT] = value;
                break;
            case GET_SIGNATURES:
                if(value>time[GET_SIGNATURES])
                time[GET_SIGNATURES] = value;
                break;
            case FIND_SIMILARITY:
                if(value>time[FIND_SIMILARITY])
                time[FIND_SIMILARITY] = value;
                break;
            case GET_SKETCHES:
                if(value>time[GET_SKETCHES])
                time[GET_SKETCHES] = value;
                break;
            case CREATE_TRIPLETS:
                if(value>time[CREATE_TRIPLETS] )
                time[CREATE_TRIPLETS] = value;
                break;
            case DO_CLUSTERING:
                if(value>time[DO_CLUSTERING])
                time[DO_CLUSTERING] = value;
                break;
            case MERGE_SORT:
                if(value>time[MERGE_SORT])
                time[MERGE_SORT] = value;
                break;
            case TOTAL_TIME:
                if(value>time[TOTAL_TIME])
                time[TOTAL_TIME] = value;
                break;
            default:
                break;
        }
    }
    else if(task == EXPORT_LOG){

        char buffer[100];
        int numb_of_threads = value;
        char *filename = "pthread_time_log.txt";
        FILE *fp = fopen(filename, "a");
        sprintf(buffer, "Numero di threads: %d\n\n Elapsed times: \n\n", numb_of_threads);
        fwrite(buffer, strlen(buffer), 1, fp);

        fprintf(fp, "MAIN:                      %.4f \n\n", time[MAIN]);
        fprintf(fp, "LIST_DIR:                  %.4f \n\n", time[LIST_DIR]);
        fprintf(fp, "COUNT_NUMBER_OF_FILES:     %.4f \n\n", time[COUNT_NUMBER_OF_FILES]);
        fprintf(fp, "GET_FILE_STRINGS_CLEANED:  %.4f \n\n", time[GET_FILE_STRINGS_CLEANED]);
        fprintf(fp, "COMPRESS_SPACES:           %.4f \n\n", time[COMPRESS_SPACES]);
        fprintf(fp, "SHINGLE_EXTRACT:           %.4f \n\n", time[SHINGLE_EXTRACT]);
        fprintf(fp, "GET_SIGNATURES:            %.4f \n\n", time[GET_SIGNATURES]);
        fprintf(fp, "FIND_SIMILARITY:           %.4f \n\n", time[FIND_SIMILARITY]);
        fprintf(fp, "GET_SKETCHES:              %.4f \n\n", time[GET_SKETCHES]);
        fprintf(fp, "CREATE_TRIPLETS:           %.4f \n\n", time[CREATE_TRIPLETS]);
        fprintf(fp, "DO_CLUSTERING:             %.4f \n\n", time[DO_CLUSTERING]);
        fprintf(fp, "MERGE_SORT:                %.4f \n\n", time[MERGE_SORT]);
        fprintf(fp, "TOTAL_TIME:                %.4f \n\n", time[TOTAL_TIME]);

        //salva i tempi dettagliati nel file TXT
        sprintf(buffer, "#############pthread############# \n\n");
        fwrite(buffer, strlen(buffer), 1, fp);
        printf("--> Tempi di esecuzione salvati in \"pthread_time_log.txt e in pthread_time_log.csv\"\n\n");
        fclose(fp);

        //salva i tempi nel file CSV
        fp = fopen("pthread_time_log.csv", "a");
        //csv format: functions time 1-12 + numb_of_threads
        for(int i=0; i<NUMBER_OF_FUNCTIONS;i++)
            fprintf(fp, "%.4f,", time[i]);
        fprintf(fp, "%d\n", numb_of_threads
        );
        fclose(fp);

    }


}

void check_coherence(long long unsigned **minhashDocumenti, int numberOfFiles){

   
    FILE *results_pthreads = fopen("results_pthreads.txt", "w+");

    for(int i=0; i<numberOfFiles; i++)
        for(int j=0; j<200;j++){
            fprintf(results_pthreads, "%llu\n", minhashDocumenti[i][j]);

        }


    fclose(results_pthreads);

}



double getElapsedTime(struct timespec *begin, struct timespec *end){
    clock_gettime(CLOCK_REALTIME, end);
    long seconds = end->tv_sec - begin->tv_sec;
    long nanoseconds = end->tv_nsec - begin->tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
    return elapsed;
}