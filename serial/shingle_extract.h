
#define K_SHINGLE 54


int shingle_extract_buf(FILE * fp, const long long filesize, char **shingles);

void print_shingles(long long tot_shingles, char **shingles);

char** get_shingles(FILE *fp, long long *tot_shingles);