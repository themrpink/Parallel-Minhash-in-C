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



int list_dir(const char *nomeDirectory, char **files, int *numberOfFiles ) {
    numberOfFiles=0;


    if (!exists(nomeDirectory) && !isDirectory(nomeDirectory)) {
        return 0;
    } else {
        DIR *elemento;
        if ((elemento = opendir(nomeDirectory)) == NULL) {
            return 0;
        }
        struct dirent *entry;
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
            int i=numberOfFiles;
            char (*tmp)[i] =realloc(files,((i+1) * PATH_MAX)*sizeof (char));
            numberOfFiles++;
            i=numberOfFiles;
            if (tmp != NULL)
            {
                files = tmp;
                strcpy(files[i],path);
            }
        }
        if (closedir(elemento) != 0) {
            exit(EXITSYSCALLFAIL);
        }
    }

    return PATH_MAX;
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

