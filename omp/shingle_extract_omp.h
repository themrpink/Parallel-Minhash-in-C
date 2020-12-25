#include <omp.h>

#define BUF 1000   //each thread reads 8kb (8000 bytes) from buffer
#define K_SHINGLE 63

void print_shingles(int tot_shingles, char **shingles);

/*
    la versione più semplice e efficiente, con un parallel for.
*/
int shingle_extract_buf_r(FILE * fp, const long filesize, char **shingles, int thread_count);

/*
    work in progress
    implementazione per  pthreads, ogni thread prende un buffer ed esegue questa funzione su quel buffer
    se si tratta dell'ultimo thread andrà completa la fine degli shingles, se si tratta del primo thread va iniziata.
*/
int shingle_extract_pthread(char *buffer, const long filesize, char **shingles);

int shingle_extract_buf(FILE * fp, const long filesize, char **shingles);

char** get_shingles(FILE *fp, long *tot_shingles, int  thread_count);

long shingle_extract_buf_hashmap(FILE * fp, const long filesize, char **shingles, int thread_count);

char ** get_shingles_serial(FILE *fp,  long *tot_shingles, int thread_count);

char ** get_shingles_hashmap(FILE *fp,  long *tot_shingles, int thread_count);