//
// Created by dani2612 on 19/12/20.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>
#include "documents_getters.h"
#include <pthread.h>
#include "time_test.h"

#define NUMB_THREADS 4

//gcc -Wall -fopenmp -lpthread -o main main.c documents_getters.c get_similarities.c hash_FNV_1.c  tokenizer.c shingle_extract.c time_test.c


int list_dir(char *nomeDirectory, char ***files) {

        if (!exists(nomeDirectory) && !isDirectory(nomeDirectory))
            return 0;

        DIR *elemento;
        if ((elemento = opendir(nomeDirectory)) == NULL)
            return 0;

        int numberOfFiles=0;
        numberOfFiles=countNumberOfFiles(nomeDirectory,elemento);
        *files = (char**)calloc(numberOfFiles, sizeof(char*));

        int thread_count = NUMB_THREADS;
        if(NUMB_THREADS>numberOfFiles)
            thread_count = numberOfFiles;
        pthread_t *thread_handles;
        thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
        GetFileNameParameters argomenti[thread_count];
        for (int i = 0; i < thread_count; ++i) {
            argomenti[i].elemento = elemento;
            argomenti[i].nomeDirectory = nomeDirectory;
            argomenti[i].files = files;
            argomenti[i].numberOfFiles = numberOfFiles;
            argomenti[i].numberOfThreads = thread_count;
            argomenti[i].rank=i;
        }
        int thread;

    for (thread=0; thread< thread_count; thread++){
            pthread_create(&thread_handles[thread], NULL, getFileName, (void*) &argomenti[thread]);
        }


        for ( thread = 0; thread<thread_count; thread++)
            pthread_join(thread_handles[thread], NULL);
    if (closedir(elemento) != 0) {
        exit(EXITSYSCALLFAIL);
    }
    return numberOfFiles;
}

int exists(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        if (ENOENT == errno) {
            return 0;
        }
    }
    return 1;
}

int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

int isRegularFile(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISREG(statbuf.st_mode);;
}

int countNumberOfFiles(const char *nomeDirectory,DIR *elemento) {
    int numberOfFiles=0;
    struct dirent *entry;
    while ((entry = readdir(elemento)) != NULL) {
        const char *figlio;
        figlio = entry->d_name; //
        if (strcmp(figlio, ".") == 0 || strcmp(figlio, "..") == 0) {
            continue;
        }
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s/%s", nomeDirectory,
                 figlio);
        if (!isRegularFile(path)) {
            continue;
        }
        numberOfFiles++;
    }
    rewinddir(elemento);
    return numberOfFiles;
}


void *getFileName(void* args){
    struct dirent *entry;
    GetFileNameParameters *arg = (GetFileNameParameters *) args;
    int rank = arg->rank;
    int numberOfFiles = arg->numberOfFiles;
    int start_loop = (numberOfFiles / arg->numberOfThreads) * rank;
    int end_loop = (numberOfFiles / arg->numberOfThreads) * (rank+1);
    if( rank+1 == arg->numberOfThreads )
        end_loop = numberOfFiles;

    //int index=start_loop;
    for(int i=start_loop; i<end_loop; i++) {
        if ((entry = readdir(arg->elemento)) != NULL) {
            const char *figlio;
            figlio = entry->d_name; //
            if (strcmp(figlio, ".") == 0 || strcmp(figlio, "..") == 0) {
                --i;
                continue;
            }
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s/%s", arg->nomeDirectory, figlio);
            if (!isRegularFile(path)) {
                continue;
            }
            (*(arg->files))[i] = strdup(path);
        }
    }
    return 0;
}