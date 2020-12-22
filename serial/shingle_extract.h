

#define PRIMES_SIZE 200
#define K_SHINGLE 54


int shingle_extract_buf(FILE * fp, const long filesize, char **shingles);
void print_shingles(int tot_shingles, char **shingles);