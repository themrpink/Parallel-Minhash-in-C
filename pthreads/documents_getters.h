#ifndef MINHASHPROJECT_GET_DOCUMENTS_H
#define MINHASHPROJECT_GET_DOCUMENTS_H
#include <dirent.h>
int list_dir(const char *nomeDirectory,char ***files);

int exists(const char *path);

int isDirectory(const char *path);

int isRegularFile(const char *path);

int countNumberOfFiles(const char *nomeDirectory,DIR *elemento);

#endif //MINHASHPROJECT_GET_DOCUMENTS_H


#define EXITSYSCALLFAIL 50