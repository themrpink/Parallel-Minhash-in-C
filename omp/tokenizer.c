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
   int stringLength=strlen(str);
   int deleted=0;
   #pragma parallel for
   for (int i = 0; i < stringLength; ++i) {
       if (isspace(str[i])){
           #pragma atomic
           deleted++;
       }

   }
    int indexToDelete[deleted];
    deleted=0;
    #pragma parallel for
    for (int i = 0; i < stringLength; ++i) {
        if (isspace(str[i])){
            #pragma critical
            {
            indexToDelete[deleted]=i;
            deleted++;
            }
        }
    }

    for (int i = 0; i < deleted; ++i) {
        removeAt(str,indexToDelete[i]);
    }



}
void removeAt(char* str, int idx) {
    size_t len = strlen(str);
    memmove(str + idx, str + idx + 1, len - idx);
}

