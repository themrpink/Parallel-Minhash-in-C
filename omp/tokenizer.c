#include "tokenizer.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include <stdlib.h>
#include <omp.h>

#define EMPTY ""


char* get_file_string_cleaned(const char* file_path,long* fileLength){
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
        #pragma parallel for
        for (i = 0; i < s; i++)
            testo[i] = tolower(testo[i]);
        *fileLength=i;
       return testo;
    }else{
        *fileLength=0;
        return EMPTY;
    }
}

void compress_spaces(char *str){
    char *tmp=malloc(strlen(str)*sizeof (char));
    int lunghezzaOriginaria=strlen(str);
    #pragma parallel  for
    for (int i = 0; (i+1) < lunghezzaOriginaria; ++i) {
        if(!(isspace(str[i]) && isspace(str[i+1]))){
            tmp[i]=str[i];
        }else{
            tmp[i]=0;
        }
    }

    int j=0;
    #pragma parallel for
    for (int i = 0; i < lunghezzaOriginaria; ++i) {
        if (tmp[i]!=0){
            #pragma atomic
            j++;
            str[j]=tmp[i];
        }
    }
    j++;
    str[j]=0;
    free(tmp);
}