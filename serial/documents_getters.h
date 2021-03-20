#ifndef MINHASHPROJECT_GET_DOCUMENTS_H
#define MINHASHPROJECT_GET_DOCUMENTS_H
#include <dirent.h>


/**
 *  restituisce una lista dei files validi nella directory
 * @param nomeDirectory
 * @param files
 * @return
 */
int list_dir(const char *nomeDirectory,char ***files);
/**
 *  controlla se il file esiste
 * @param path
 * @return
 */
int exists(const char *path);
/**
 * controlla se il file è un directory
 * @param path
 * @return
 */
int isDirectory(const char *path);
/**
 * controlla se il file è regolare
 * @param path
 * @return
 */
int isRegularFile(const char *path);
/**
 * restituisce il numero di files nella directory
 * @param nomeDirectory
 * @param elemento
 * @return
 */
int countNumberOfFiles(const char *nomeDirectory,DIR *elemento);
#endif //MINHASHPROJECT_GET_DOCUMENTS_H


#define EXITSYSCALLFAIL 50