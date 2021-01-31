//
// Created by dani2612 on 19/12/20.
//
#ifndef MINHASHPROJECT_TOKENIZER_H
#define MINHASHPROJECT_TOKENIZER_H

typedef struct {
    char * testo;
    int string_length;
    int number_of_threads;
    int rank;
}  lower_case_arg;

char* get_file_string_cleaned(const char* file_path,long* fileLength);
int compress_spaces(char *str, long length);
void removeAt(char* str, int idx);
void *lower_case( void* args);
#endif //MINHASHPROJECT_TOKENIZER_H
