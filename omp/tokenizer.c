#include "tokenizer.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include <stdlib.h>
#include <omp.h>
#include "time_test.h"
#define EMPTY ""



char* get_file_string_cleaned(const char* file_path,long* fileLength){
    double start;
    double  end;
    start = omp_get_wtime();

    long length=0;
    char * testo;// = 0;
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

        int i, s = strlen(testo);
        
        #pragma omp parallel for num_threads(global_thread_numb) schedule(auto)
        for (i = 0; i < s; i++)
            testo[i] = tolower(testo[i]);
        *fileLength=s;
        end = omp_get_wtime();
        exectimes(end-start, GET_FILE_STRINGS_CLEANED, SET_TIME);

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