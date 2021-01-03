
#include "tokenizer.h"
#include "stdio.h"
#include "ctype.h"

#define EMPTY ""

char* get_file_string_cleaned(const char* file_path,long* fileLength){
    char * buffer = 0;
    FILE * fp = fopen (file_path, "rb");
    if (fp){
        fseek (fp, 0, SEEK_END);
        fileLength = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
        {
            fread (buffer, 1, fileLength, fp);
        }
        fclose (f);
    }
    if (buffer){
       compress_spaces(testo);
       testo=tolower(testo);
       return testo;
    }else{
        fileLength=0;
        return EMPTY;
    }
}

void compress_spaces(char *str){
    char *dst = str;
    for (; *str; ++str) {
        *dst++ = *str;

        if (isspace(*str)) {
            do ++str;
            while (isspace(*str));

            --str;
        }
    }
    *dst = 0;
}