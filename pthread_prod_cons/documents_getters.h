#ifndef MINHASHPROJECT_GET_DOCUMENTS_H
#define MINHASHPROJECT_GET_DOCUMENTS_H
#include <dirent.h>



typedef struct GetFileNameParameters{
    char *nomeDirectory;
    DIR *elemento;
    char ***files;
    int numberOfFiles;
    int numberOfThreads;
    int rank;
    struct Prod_Cons_Data *files_struct;
} GetFileNameParameters;

typedef struct node {
    char *data;
    struct node *next;
} linked_list_t;

/*
    restituisce una lista dei files validi nella directory
*/
void *list_dir(void * args);


/*
 controlla se il file esiste
*/
int exists(const char *path);


/*
 controlla se il file è un directory
*/
int isDirectory(const char *path);

/*
    controlla se il file è regolare
*/
int isRegularFile(const char *path);

/*
    restituisce il numero di files nella directory
*/
int countNumberOfFiles(const char *nomeDirectory);

/*
    funzione che restituisce i nomi dei file
*/
void *getFileName(void* args);

#endif //MINHASHPROJECT_GET_DOCUMENTS_H


#define EXITSYSCALLFAIL 50