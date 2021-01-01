#include "get_similarities.h"
#include <stdlib.h>


int get_sketches(int i, struct sign_doc *file_sketches, long long unsigned *signatures, char *filename){

   for(int s=0; s<N_SIGNATURES; s++){
      file_sketches[s].doc_id = i;
      file_sketches[s].signature = *(signatures+s);
   }

    return 0;
}





int find_similarity(int numberOfFiles, char **files, long long unsigned **minhashDocumenti, struct doc_couple* couples ){

   couples = (struct doc_couple*) malloc( numberOfFiles *numberOfFiles* 200*sizeof(struct doc_couple));
   struct sign_doc* files_sketches = (struct sign_doc*) malloc(numberOfFiles * N_SIGNATURES * sizeof(struct sign_doc));
   struct sign_doc* temp_sketches = (struct sign_doc*) malloc(numberOfFiles * N_SIGNATURES * sizeof(struct sign_doc));

   for(int i=0; i < numberOfFiles; i++)
      get_sketches(i, files_sketches+(i*N_SIGNATURES) , minhashDocumenti[i], files[i]);

   mergesort_s_signatures(files_sketches, N_SIGNATURES*numberOfFiles, temp_sketches);
   free(temp_sketches);

   //crea le triple {doc1, doc2, shared_signatures}
   int count = create_triplets(files_sketches, numberOfFiles, couples);

   //copia di appoggio per il mergesort
   struct doc_couple* couples_tmp = (struct doc_couple*) malloc( count * sizeof(struct doc_couple));

   //ordina per doc_id
   mergesort_s_doc_id(couples, count, couples_tmp);

   //raccogli le coppie di documenti che hanno almeno una signature in comune
   int index = do_clustering(couples, count);
    
   return index;
 
}





int create_triplets(struct sign_doc* files_sketches, int numberOfFiles, struct doc_couple* couples){

   int count=0;
   long tot = numberOfFiles*N_SIGNATURES;
   long long unsigned signature_curr;

   for(int i=0; i < tot ; i++){
      signature_curr = files_sketches[i].signature;
      int signature_succ = i+1;

      //fino a che la signature successiva è uguale alla signature corrente:
      while(signature_succ < tot && files_sketches[signature_succ].signature == signature_curr){

        //inserisci in couples una tripla che contiene una coppia di doc_id ed un "1" a indicare che questi condividono una signature
         couples[count].doc_id = files_sketches[i].doc_id;
         couples[count].doc2_id = files_sketches[signature_succ].doc_id;
         couples[count].shared_signatures = 1;
         signature_succ++;

         //questo count servirà più avanti per sommare le signature condivise
         count++;            
      }
   }


    /*
        ridimensiona l'array con ai primi "count" elementi (tutti quelli cioè che hanno shared_signatures=1) 
    */
   couples = (struct doc_couple*) realloc( couples, (--count) * sizeof(struct doc_couple));


   free(files_sketches);
   return count;
}






int do_clustering(struct doc_couple* couples, int count){

   int index=0;
   for(int i=0; i<count; i++){
      if( couples[i].shared_signatures!=0){
         for(int j=i; j<count; j++){
            if (couples[i].doc_id == couples[j].doc_id && couples[i].doc2_id == couples[j].doc2_id){
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
   couples = (struct doc_couple*) realloc( couples, (--index) * sizeof(struct doc_couple));

   for(int i=0; i<=index; i++){
        printf("\n%d) shared_signatures: %d", i, couples[i].shared_signatures);
        printf("%15s   doc_id_1:  %llu"," ", couples[i].doc_id);
        printf("%15s   doc_id_2:  %llu "," ", couples[i].doc2_id);
        printf("%15s   similarity  %.3f\n"," ", (float)couples[i].shared_signatures/400.0);
   }


   return index;
}



void merge_doc_id(struct doc_couple* X, int n, struct doc_couple*  tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;

   while (i<n/2 && j<n) {
      if (X[i].doc_id < X[j].doc_id ) {
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
   memcpy(X, tmp, n*sizeof(struct doc_couple));
} 


void merge_signatures(struct sign_doc* X, int n, struct sign_doc*  tmp) {
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
   while (i<n/2) { /* prima metà */
      tmp[ti] = X[i];
      ti++; i++;
   }
   while (j<n) { /* seconda metà */
      tmp[ti] = X[j];
      ti++; j++;
   }
   memcpy(X, tmp, n*sizeof(struct sign_doc));
} 



void mergesort_s_signatures(struct sign_doc* X, int n, struct sign_doc* tmp)
{
   if (n < 2) return;

   #pragma omp task firstprivate (X, n, tmp)
   mergesort_s_signatures(X, n/2, tmp);

   //#pragma omp task firstprivate (X, n, tmp)
   mergesort_s_signatures(X+(n/2), n-(n/2), tmp);
 
   #pragma omp taskwait 
   merge_signatures(X, n, tmp);
}

void mergesort_s_doc_id(struct doc_couple* X, int n, struct doc_couple* tmp)
{
   if (n < 2) return;

  // #pragma omp task firstprivate (X, n, tmp)
   mergesort_s_doc_id(X, n/2, tmp);

//   #pragma omp task firstprivate (X, n, tmp)
   mergesort_s_doc_id(X+(n/2), n-(n/2), tmp);
 
 //  #pragma omp taskwait
   merge_doc_id(X, n, tmp);
}