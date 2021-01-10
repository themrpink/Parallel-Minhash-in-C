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

    char * testo = 0;
    FILE * fp = fopen (file_path, "rb");
    if (fp){
        fseek (fp, 0, SEEK_END);
        long length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        testo = malloc (length);
        if (testo)
        {
            fread (testo, 1, length, fp);
        }
        fclose (fp);
    }
    if (testo){
       compress_spaces(testo);
        int i, s = strlen(testo);

        start = omp_get_wtime();
        //#pragma omp parallel for
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

void compress_spaces(char *str){
    double start;
    double  end;
    double elapsed;

    char *tmp=malloc(strlen(str)*sizeof (char));
    int lunghezzaOriginaria=strlen(str);

    start=omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i <= lunghezzaOriginaria; ++i) {
        if(!(isspace(str[i]) && isspace(str[i+1]))){
            tmp[i]=str[i];
        }else{
            tmp[i]=0;
        }
    }
    end = omp_get_wtime();
    elapsed = end - start;

    int j=0;
    start=omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < lunghezzaOriginaria; ++i) {
        if (tmp[i]!=0){
           // #pragma omp critical
            {
            j++;
            str[j]=tmp[i];
            }

        }
    }
    end = omp_get_wtime();
    elapsed += (end-start);
    exectimes(elapsed, COMPRESS_SPACES, SET_TIME);

    j++;
    str[j]=0;
    
//    str=realloc(str,(j+1)*sizeof (char));
    free(tmp);
}