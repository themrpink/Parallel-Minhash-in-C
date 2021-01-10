
#ifndef TIME_TEST_  /* Include guard */
#define TIME_TEST_

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
    NUMBER_OF_FUNCTIONS
};

void exectimes(double value, enum Function_name function_name, enum Task task);
void check_coherence(long long unsigned **minhashDocumenti, int numberOfFiles);

#endif