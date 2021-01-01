#ifndef MINHASHPROJECT_GET_DOCUMENTS_H
#define MINHASHPROJECT_GET_DOCUMENTS_H
int list_dir(const char *nomeDirectory,char **files,int *numberOfFiles );

int exists(const char *path);

int isDirectory(const char *path);

int isRegularFile(const char *path);

#endif //MINHASHPROJECT_GET_DOCUMENTS_H


#define EXITSYSCALLFAIL 50