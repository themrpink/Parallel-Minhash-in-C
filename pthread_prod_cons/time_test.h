
#ifndef TIME_TEST_  /* Include guard */
#define TIME_TEST_

#include <time.h>

enum Task{
    SET_TIME,
    EXPORT_LOG
};

enum Function_name {
    MAIN,
    LIST_DIR,
    COUNT_NUMBER_OF_FILES,
    GET_FILE_STRINGS_CLEANED,
    COMPRESS_SPACES,
    SHINGLE_EXTRACT,
    GET_SIGNATURES,
    FIND_SIMILARITY,
    GET_SKETCHES,
    CREATE_TRIPLETS,
    DO_CLUSTERING,
    MERGE_SORT,
    TOTAL_TIME,
    NUMBER_OF_FUNCTIONS
};


/*
    raccoglie i tempi di tutte le funzioni (specificate nel terzo parametro).
    Utilizzando come terzo parametro EXPORT_LOG, salva su file "time_log.txt" e "time_log.csv" tutti i tempi.
*/
void exectimes(double value, enum Function_name function_name, enum Task task);


/*
    esegue dei controlli per verificare che i risultati del programma se eseguito piú volte sugli stessi documenti dia
    risultati coerenti.
*/
void check_coherence(long long unsigned **minhashDocumenti, int numberOfFiles);
double getElapsedTime(struct timespec *begin, struct timespec *end);

#endif