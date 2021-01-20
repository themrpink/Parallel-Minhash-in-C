//
// Created by dani2612 on 19/12/20.
//
#ifndef MINHASHPROJECT_TOKENIZER_H
#define MINHASHPROJECT_TOKENIZER_H
char* get_file_string_cleaned(const char* file_path,long* fileLength);
int compress_spaces(char *str,long length);
void removeAt(char* str, int idx);
#endif //MINHASHPROJECT_TOKENIZER_H
