#include "time_test.h"
#include <stdio.h>
#include <omp.h>
#include <string.h>


void exectimes(double value, enum Function_name function_name, enum Task task){

    static double time[NUMBER_OF_FUNCTIONS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    static int t0 = 1;
    static int t1 = 1;
    static int t2 = 1;
    static int t3 = 1;
    static int t4 = 1;
    static int t5 = 1;
    static int t6 = 1;
    static int t7 = 1;
    static int t8 = 1;
    static int t9 = 1;
    static int t10 = 1;
    static int t11 = 1;

    if(task == SET_TIME) {
        switch (function_name) {
            case MAIN:
                time[MAIN] = value;
                t0+=1;
                break;
            case LIST_DIR:
                time[LIST_DIR] = value;
                t1+=1;
                break;
            case COUNT_NUMBER_OF_FILES:
                time[COUNT_NUMBER_OF_FILES] = value;
                t2+=1;
                break;
            case GET_FILE_STRINGS_CLEANED:
                time[GET_FILE_STRINGS_CLEANED] = value;
                t3+=1;
                break;
            case COMPRESS_SPACES:
                time[COMPRESS_SPACES] = value;
                t4+=1;
                break;
            case SHINGLE_EXTRACT:
                time[SHINGLE_EXTRACT] = value;
                t5+=1;
                break;
            case GET_SIGNATURES:
                time[GET_SIGNATURES] = value;
                t6+=1;
                break;
            case FIND_SIMILARITY:
                time[FIND_SIMILARITY] = value;
                t7+=1;
                break;
            case GET_SKETCHES:
                time[GET_SKETCHES] = value;
                t8+=1;
                break;
            case CREATE_TRIPLETS:
                time[CREATE_TRIPLETS] = value;
                t9+=1;
                break;
            case DO_CLUSTERING:
                time[DO_CLUSTERING] = value;
                t10+=1;
                break;
            case MERGE_SORT:
                time[MERGE_SORT] = value;
                t11+=1;
                break;
            default:
                break;
        }
    }
    else if(task == EXPORT_LOG){

        char buffer[100];
        int numb_of_threads = value;
        char *filename = "time_log.txt";
        FILE *fp = fopen(filename, "a");
        sprintf(buffer, "Number of threads: %d\n\n Elapsed times: \n\n", numb_of_threads);
        fwrite(buffer, strlen(buffer), 1, fp);

        for (int i=0; i<NUMBER_OF_FUNCTIONS; i++) {
            switch (i) {
                case 0:
                    sprintf(buffer, "MAIN:  %.3f \n\n", time[i]);
                    break;
                case 1:
                    sprintf(buffer, "LIST_DIR:  %.3f \n\n", time[i]);
                    break;
                case 2:
                    sprintf(buffer, "COUNT_NUMBER_OF_FILES:  %.3f \n\n", time[i]);
                    break;
                case 3:
                    sprintf(buffer, "GET_FILE_STRINGS_CLEANED:  %.3f \n\n", time[i]);;
                    break;
                case 4:
                    sprintf(buffer, "COMPRESS_SPACES:  %.3f \n\n", time[i]);
                    break;
                case 5:
                    sprintf(buffer, "SHINGLE_EXTRACT:  %.3f \n\n", time[i]);
                    break;
                case 6:
                    sprintf(buffer, "GET_SIGNATURES:  %.3f \n\n", time[i]);
                    break;
                case 7:
                    sprintf(buffer, "FIND_SIMILARITY:  %.3f \n\n", time[i]);
                    break;
                case 8:
                    sprintf(buffer, "GET_SKETCHES:  %.3f \n\n", time[i]);
                    break;
                case 9:
                    sprintf(buffer, "CREATE_TRIPLETS  %.3f \n\n", time[i]);
                    break;
                case 10:
                    sprintf(buffer, "DO_CLUSTERING:  %.3f \n\n", time[i]);;
                    break;
                case 11:
                    sprintf(buffer, "MERGE_SORT:  %.3f \n\n", time[i]);
                    break;
                default:
                    break;
            }

            fwrite(buffer, strlen(buffer), 1, fp);
        }
        sprintf(buffer, "################################# \n\n");
        fwrite(buffer, strlen(buffer), 1, fp);
        printf("--> Tempi di esecuzione salvati in \"time_log.txt\"\n\n");
        fclose(fp);
    }
}

void check_coherence(long long unsigned **minhashDocumenti, int numberOfFiles){

    FILE *results_serial = fopen("results_serial.txt", "r");
    FILE *results_omp = fopen("results_omp.txt", "w+");

    for(int i=0; i<numberOfFiles; i++)
        for(int j=0; j<200;j++)
            fprintf(results_omp, " %llu", minhashDocumenti[i][j]);
    rewind(results_omp);
    if (results_serial == NULL){
        printf("--> ERRORE: File seriale mancante:\nrieseguire il programma con un solo thread, rinominare il file \"results_omp.txt\" in \"results_serial.txt\" ed eseguire di nuovo in parallelo\n\n");
        return;
    }

    int count = 0;
    int c1;

    //confronta i due file
    while ((c1 = fgetc(results_omp)) != EOF) {
       if(c1!= fgetc(results_serial))
        count++;
    }

    if(count==0)
        printf("--> OK, nessun problema di coerenza tra signatures\n");
    else
        printf("--> ERRROE: \nProblema di coerenza delle signatures: sono diverse almeno %d volte\nControllare i file results_serial.txt e results_omp.txt", count);
    fclose(results_omp);
    fclose(results_serial);


}

