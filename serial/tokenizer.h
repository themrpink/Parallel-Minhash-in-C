//
// Created by dani2612 on 19/12/20.
//
#ifndef MINHASHPROJECT_TOKENIZER_H
#define MINHASHPROJECT_TOKENIZER_H

/*
    Dato un file lo tokenizza e restituisce la nuova dimensione tokenizzata in bytes.
*/
char* get_file_string_cleaned(const char* file_path,long* fileLength);

/*
    Funzione che tokenizza eliminando gli spazi, i caratteri non ASCII, i tab, ecc.
*/
int compress_spaces(char *str,long length);

#endif //MINHASHPROJECT_TOKENIZER_H
