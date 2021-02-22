#ifndef MINHASHPROJECT_GET_DOCUMENTS_H
#define MINHASHPROJECT_GET_DOCUMENTS_H
#include <dirent.h>


/*
    restituisce una lista dei files validi nella directory
*/
int list_dir(const char *nomeDirectory,char ***files);

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
int countNumberOfFiles(const char *nomeDirectory,DIR *elemento);
#endif //MINHASHPROJECT_GET_DOCUMENTS_H


#define EXITSYSCALLFAIL 50