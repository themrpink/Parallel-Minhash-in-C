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
#include <time.h>
#include <pthread.h>
#include "documents_getters.h"
#include "time_test.h"
#include "queue.h"
#include "prod_cons.h"


#define NUMB_THREADS 2

static int totNumbOfFiles = 0;
/*
void *list_dir(void * args) {

    args_list_dir* args_list = (args_list_dir*) args;
    Prod_Cons_Data *files_struct = (Prod_Cons_Data *)args_list->files_struct;
  //  char ***files = args_list->files;
    char *nomeDirectory =  args_list->nomeDirectory;
    int numberOfFiles = args_list->numberOfFiles;;

    if (!exists(nomeDirectory) && !isDirectory(nomeDirectory))
        return 0;
    DIR *elemento;
    if ((elemento = opendir(nomeDirectory)) == NULL)
        return 0;
    
   // *files = (char**)calloc(numberOfFiles, sizeof(char*));

    int thread_count = NUMB_THREADS;
    if(NUMB_THREADS>numberOfFiles)
        thread_count = numberOfFiles;

    pthread_t *thread_handles;
    thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    GetFileNameParameters getFileName_args[thread_count];
    int thread;

    for (thread=0; thread< thread_count; thread++){
        getFileName_args[thread].elemento = elemento;
        getFileName_args[thread].nomeDirectory = nomeDirectory;
     //   getFileName_args[thread].files = files;
        getFileName_args[thread].numberOfFiles = numberOfFiles;
        getFileName_args[thread].numberOfThreads = thread_count;
        getFileName_args[thread].files_struct = files_struct;
        getFileName_args[thread].rank = thread;
        pthread_create(&thread_handles[thread], NULL, getFileName, (void*) &getFileName_args[thread]);
    }

    for ( thread = 0; thread<thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    if (closedir(elemento) != 0) {
            exit(EXITSYSCALLFAIL);
        }
    
    return 0;
}
*/
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

int countNumberOfFiles(const char *nomeDirectory) {
    DIR *elemento;
    if ((elemento = opendir(nomeDirectory)) == NULL)
        return 0;

    int numberOfFiles=0;
    struct dirent *entry;
    while ((entry = readdir(elemento)) != NULL) {
        totNumbOfFiles++;
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

/*
void *getFileName(void* args){  
    struct dirent *entry;
    GetFileNameParameters *arg = (GetFileNameParameters *) args;
    Prod_Cons_Data *files_struct = (Prod_Cons_Data *) arg->files_struct;
    int rank = arg->rank;
    
    int start_loop = (totNumbOfFiles / arg->numberOfThreads) * rank;
    int end_loop = (totNumbOfFiles / arg->numberOfThreads) * (rank+1);
    
    if( rank+1 == arg->numberOfThreads )
        end_loop = totNumbOfFiles;

    for(int i=start_loop; i<end_loop; i++) {
        if ((entry = readdir(arg->elemento)) != NULL) {
            const char *figlio;
            figlio = entry->d_name; 
            
            if (strcmp(figlio, ".") == 0 || strcmp(figlio, "..") == 0) {
                continue;
            }
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s/%s", arg->nomeDirectory, figlio);

            if (!isRegularFile(path)) {
                continue;
            }

            producer(files_struct, (void*)strdup(path));
        }

    }
    return 0;
}

*/

void *list_dir(void * args) {

    args_list_dir* args_list = (args_list_dir*) args;
    Prod_Cons_Data *files_struct = (Prod_Cons_Data *)args_list->files_struct;

    char *nomeDirectory =  args_list->nomeDirectory;
    int numberOfFiles = args_list->numberOfFiles;;

    if (!exists(nomeDirectory) && !isDirectory(nomeDirectory))
        return 0;

    DIR *elemento;
    if ((elemento = opendir(nomeDirectory)) == NULL)
        return 0;
    
    struct dirent *entry;
    const char *figlio;
    char path[PATH_MAX];

    for(int i=0; i<numberOfFiles; i++) {
        if ((entry = readdir(elemento)) != NULL) {
            
            figlio = entry->d_name; 
            
            if (strcmp(figlio, ".") == 0 || strcmp(figlio, "..") == 0) {
                numberOfFiles++;
                continue;
            }
            
            snprintf(path, PATH_MAX, "%s/%s", nomeDirectory, figlio);

            if (!isRegularFile(path)) {
                numberOfFiles++;
                continue;
            }
         
            producer(files_struct, (void*)strdup(path));
        }

    }

    if (closedir(elemento) != 0) {
            exit(EXITSYSCALLFAIL);
        }

}
