#ifndef MINHASHPROJECT_GET_DOCUMENTS_H
#define MINHASHPROJECT_GET_DOCUMENTS_H
#include <dirent.h>



typedef struct  {
    char *nomeDirectory;
    DIR *elemento;
    char ***files;
    int numberOfFiles;
    int numberOfThreads;
    int rank;
}GetFileNameParameters;

typedef struct node {
    char *data;
    struct node *next;
} linked_list_t;


int list_dir(char *nomeDirectory,char ***files);

int exists(const char *path);

int isDirectory(const char *path);

int isRegularFile(const char *path);

int countNumberOfFiles(const char *nomeDirectory,DIR *elemento);

void *getFileName(void* args);

#endif //MINHASHPROJECT_GET_DOCUMENTS_H


#define EXITSYSCALLFAIL 50