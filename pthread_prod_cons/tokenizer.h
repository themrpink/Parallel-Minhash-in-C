//
// Created by dani2612 on 19/12/20.
//
#ifndef MINHASHPROJECT_TOKENIZER_H
#define MINHASHPROJECT_TOKENIZER_H

struct lower_case_arg {
    char * testo;
    int string_length;
    int number_of_threads;
    int rank;
};

struct string_cleaner_args {
    const char* file_path;
    long* fileLength;
};

/*
    Dato un file lo tokenizza e restituisce la nuova dimensione tokenizzata in bytes.
*/
char* get_file_string_cleaned(const char* file_path,long* fileLength);

/*
    Funzione che tokenizza eliminando gli spazi, i caratteri non ASCII, i tab, ecc.
*/
int compress_spaces(char *str, long length);

/*
    riduce a low case
*/
void *lower_case( void* args);

#endif //MINHASHPROJECT_TOKENIZER_H
