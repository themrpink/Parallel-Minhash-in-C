#ifndef MINHASHPROJECT_SHINGLE_EXTRACT_H
#define MINHASHPROJECT_SHINGLE_EXTRACT_H
#define K_SHINGLE 54

/*
    Dato il contenuto di un documento ne estrae gli shingles.
*/
void shingle_extract_buf(char* buf, long numb_shingles, char **shingles);

#endif