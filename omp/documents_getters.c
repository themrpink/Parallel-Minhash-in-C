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
#include <omp.h>


int list_dir(const char *nomeDirectory, char ***files) {
    int numberOfFiles=0;

    if (!exists(nomeDirectory) && !isDirectory(nomeDirectory)) {
        return 0;
    } else {
        DIR *elemento;
        if ((elemento = opendir(nomeDirectory)) == NULL) {
            return 0;
        }
        struct dirent *entry;
        numberOfFiles=countNumberOfFiles(nomeDirectory,elemento);
        rewinddir(elemento);
        *files = calloc(numberOfFiles , sizeof(char*));
        int i=0;
        #pragma omp parallel
        while ((entry = readdir(elemento)) != NULL) {
            const char *figlio;
            figlio = entry->d_name; //
            if (strcmp(figlio, ".") == 0 || strcmp(figlio, "..") == 0) {
                continue;
            }

            int lunghezzaPath;
            char path[PATH_MAX];
            lunghezzaPath = snprintf(path, PATH_MAX, "%s/%s", nomeDirectory,
                                     figlio);
            if (!isRegularFile(path)) {
                continue;
            }

            #pragma omp critical
            {
                (*files)[i] = strdup(path);
                i++;
            };

        }
        if (closedir(elemento) != 0) {
            exit(EXITSYSCALLFAIL);
        }
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

int countNumberOfFiles(const char *nomeDirectory,DIR *elemento){
    int numberOfFiles=0;
    struct dirent *entry;
    #pragma omp parallel
    while ((entry = readdir(elemento)) != NULL) {
        const char *figlio;
        figlio = entry->d_name; //
        if (strcmp(figlio, ".") == 0 || strcmp(figlio, "..") == 0) {
            continue;
        }
        int lunghezzaPath;
        char path[PATH_MAX];
        lunghezzaPath = snprintf(path, PATH_MAX, "%s/%s", nomeDirectory,
                                 figlio);
        if (!isRegularFile(path)) {
            continue;
        }
        #pragma omp critical
        numberOfFiles++;
    }

    return numberOfFiles;

}