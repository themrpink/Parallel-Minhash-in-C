
#define BUF 1000   //each thread reads 8kb (8000 bytes) from buffer
#define K_SHINGLE 54

void print_shingles(int tot_shingles, char **shingles);

/*
    la versione più semplice e efficiente, con un parallel for.
*/
int shingle_extract_buf_r(FILE * fp, const long filesize, int thread_count, char **shingles);

/*
    come sopra in versione seriale
*/
int shingle_extract_buf(FILE * fp, const long filesize, int thread_count, char **shingles);


/*
    work in progress
    implementazione per  pthreads, ogni thread prende un buffer ed esegue questa funzione su quel buffer
    se si tratta dell'ultimo thread andrà completa la fine degli shingles, se si tratta del primo thread va iniziata.
*/
int shingle_extract_pthread(char *buffer, const long filesize, char **shingles);


