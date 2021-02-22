#include "tokenizer.h"
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "time_test.h"
#define EMPTY ""
#define NUMB_THREADS 2

char* get_file_string_cleaned(const char* file_path, long* fileLength){

    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    char * testo = 0;
    long length = 0;
    FILE * fp = fopen (file_path, "rb");
    if (fp){
        fseek (fp, 0, SEEK_END);
        length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        testo = malloc (length);
        if (testo)
        {
            fread (testo, 1, length, fp);
        }
        fclose (fp);
    }
    
    if (testo){
        compress_spaces(testo, length);
        int s = strlen(testo);
        *fileLength=s;
        int thread_count = NUMB_THREADS;
        if(s<thread_count)
            thread_count = s;
        pthread_t *thread_handles;
        thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
        struct lower_case_arg args[thread_count];

        int thread;
        for (thread=0; thread< thread_count; thread++){
            args[thread].rank=thread;
            args[thread].number_of_threads= thread_count;
            args[thread].string_length = s;
            args[thread].testo = testo;
            pthread_create(&thread_handles[thread], NULL, lower_case, (void*) &args[thread]);
        }

        for ( thread = 0; thread<thread_count; thread++)
            pthread_join(thread_handles[thread], NULL);
        exectimes(getElapsedTime(&begin, &end), GET_FILE_STRINGS_CLEANED, SET_TIME);
        return testo;
    }else{
        *fileLength=0;
        return EMPTY;
    }
}

int compress_spaces(char *str, long length){
    char *dst = str;
    int len=0;
    for(int i=0; i<length; i++){
        if(!isspace(*str) && *str>=0 && *str<=127){
            *dst++ = *str;
            len++;
        }
        str++;
    }
    *dst = '\0';
    return --len;
}

void *lower_case( void* args){

    struct lower_case_arg *arg = (struct lower_case_arg *) args;
    int rank = arg->rank;
    int s = arg->string_length;
    int thread_count = arg->number_of_threads;
    int start_loop = (s / thread_count) * rank;
    int end_loop = (s / thread_count) * (rank+1);
    if(rank + 1 == thread_count)
        end_loop = s;


    for (int i = start_loop; i < end_loop; i++)
        arg->testo[i] = tolower(arg->testo[i]);

    return 0;
}