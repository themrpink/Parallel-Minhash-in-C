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
#include <omp.h>
#include "time_test.h"
#include "documents_getters.h"


/**
 *  restituisce una lista dei files validi nella directory
 * @param nomeDirectory
 * @param files
 * @return
 */
int list_dir(const char *nomeDirectory, char ***files) {
    int numberOfFiles=0;
    double start;
    double  end;

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
        start = omp_get_wtime();
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


            (*files)[i] = strdup(path);
            i++;
        }
        if (closedir(elemento) != 0) {
            exit(EXITSYSCALLFAIL);
        }
        end = omp_get_wtime();
        exectimes(end-start, LIST_DIR, SET_TIME);
    }

    return numberOfFiles;
}
/**
 * controlla se il file esiste
* @param path
* @return
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
/**
 * controlla se il file è un directory
 * @param path
 * @return
 */
int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}
/**
 * controlla se il file è regolare
 * @param path
 * @return
 */
int isRegularFile(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISREG(statbuf.st_mode);;
}
/**
 * restituisce il numero di files nella directory
 * @param nomeDirectory
 * @param elemento
 * @return
 */
int countNumberOfFiles(const char *nomeDirectory,DIR *elemento){
    double start;
    double  end;
    int numberOfFiles=0;
    struct dirent *entry;
    start = omp_get_wtime();
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
        numberOfFiles++;
    }
    end = omp_get_wtime();
    exectimes(end-start, COUNT_NUMBER_OF_FILES, SET_TIME);
    return numberOfFiles;

}