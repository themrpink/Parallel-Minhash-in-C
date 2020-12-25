#include "get_similarities.h"




int get_sketches(struct sign_doc *file_sketches, long long unsigned *signatures, char *filename){

    long long unsigned hash = genera_doc_id(filename);
    struct sign_doc* temp_sketches = (struct sign_doc*)malloc(N_SIGNATURES*sizeof(struct sign_doc));

    for(int i=0; i<N_SIGNATURES; i++){
        file_sketches[i].doc_id = hash;
        file_sketches[i].signature = *(signatures+i);
    }

   //#pragma omp parallel
    {
     //   #pragma omp single
        mergesort(file_sketches, N_SIGNATURES, temp_sketches);
    }
    free(temp_sketches);

    return 0;
}





long long unsigned genera_doc_id(char *filename){

    struct stat buf;
    long long unsigned hash;
    int fd = open(filename, O_RDONLY);
    fstat(fd, &buf);
    close(fd);

    off_t tsize = buf.st_size; 
    time_t mtime = buf.st_mtime;
    char str[50];
    sprintf(str, "%li", tsize);
    strcat(str, filename);
    sprintf(str,"%ld", mtime);
    strcat(str, filename);
    hash_FNV_1a(str, &hash, strlen(str));
    FILE *fp2 = fopen("index_file.txt", "a");
    fprintf(fp2, "%llu, %s\n", hash, filename);
    
    fclose(fp2);
    return hash;
}




void find_similarity(int numberOfFiles, struct sign_doc **files_sketches){
    
   struct sign_doc* files_sketches2 = (struct sign_doc*) malloc(numberOfFiles * N_SIGNATURES * sizeof(struct sign_doc));
   struct sign_doc* temp_sketches   = (struct sign_doc*) malloc(numberOfFiles * N_SIGNATURES * sizeof(struct sign_doc));

    for(int i=0; i<numberOfFiles; i++)
        for(int j=0; j<N_SIGNATURES; j++)
            files_sketches2[i*N_SIGNATURES+j] = files_sketches[i][j];

    mergesort(files_sketches2, N_SIGNATURES*numberOfFiles, temp_sketches);

    for(int i=0; i<numberOfFiles; i++){
        free(files_sketches[i]);
    }
    free(files_sketches);
    free(temp_sketches);

    int count = 0;
    struct doc_couple* couples = (struct doc_couple*) malloc( 5000 * sizeof(struct doc_couple));
    long tot = numberOfFiles*N_SIGNATURES;

    for(int i=0; i< tot ; i++){
        long long unsigned sign = files_sketches2[i].signature;
        int j = i;
        while(j+1 < tot && files_sketches2[j+1].signature == sign){
            j++;
            couples[count].doc1_id = files_sketches2[i].doc_id;
            couples[count].doc2_id = files_sketches2[j].doc_id;
            couples[count].shared_signatures = 1;
            count++;            
        }
    }

    free(files_sketches2);

    int index=0;

    for(int i=0; i<count; i++){
        if( couples[i].shared_signatures!=0){
            for(int j=i; j<count; j++){
                if (couples[i].doc1_id == couples[j].doc1_id && couples[i].doc2_id == couples[j].doc2_id){
                    couples[i].shared_signatures+=1;
                    couples[j].shared_signatures=0;
                }
            }
            couples[index] = couples[i];
            if(index!=i)
                couples[i].shared_signatures=0;
            index+=1;
        }

    }
    couples = (struct doc_couple*) realloc( couples, index * sizeof(struct doc_couple));

    for(int i=0; i<index; i++){
        printf("%d) shared_signatures: %d", i, couples[i].shared_signatures);
        printf("%15s   doc_id_1:  %llu"," ", couples[i].doc1_id);
        printf("%15s   doc_id_2:  %llu "," ", couples[i].doc2_id);
        printf("%15s   similarity  %.3f\n"," ", (float)couples[i].shared_signatures/400.0);
    }
    
}




void merge(struct sign_doc* X, int n, struct sign_doc*  tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;

   while (i<n/2 && j<n) {
      if (X[i].signature < X[j].signature) {
         tmp[ti] = X[i];
         ti++; i++;
      } else {
         tmp[ti] = X[j];
         ti++; j++;
      }
   }
   while (i<n/2) { /* finish up lower half */
      tmp[ti] = X[i];
      ti++; i++;
   }
   while (j<n) { /* finish up upper half */
      tmp[ti] = X[j];
      ti++; j++;
   }
   memcpy(X, tmp, n*sizeof(struct sign_doc));
} 

void mergesort(struct sign_doc* X, int n, struct sign_doc* tmp)
{
   if (n < 2) return;

  // #pragma omp task firstprivate (X, n, tmp)
   mergesort(X, n/2, tmp);

  // #pragma omp task firstprivate (X, n, tmp)
   mergesort(X+(n/2), n-(n/2), tmp);
 
   //#pragma omp taskwait
   merge(X, n, tmp);
}

