#include "get_similarities.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "time_test.h"

#define N_SIGNATURES 200
#define NUMB_OF_THREADS 2

//crea le coppie doc_id - signatures (200 coppie per ogni doc con tutte le sue signatures)
int get_sketches(int i, struct sign_doc *file_sketches, long long unsigned *signatures){
    double start;
    double  end;

    for(int s=0; s<N_SIGNATURES; s++){
      file_sketches[s].doc_id = i;
      file_sketches[s].signature = *(signatures+s);

   }

   return 0;
}





int find_similarity(int numberOfFiles, char **files, long long unsigned **minhashDocumenti){
    double start;
    double  end;
    double elapsed;

    int **matrix_signatures_couples = malloc(numberOfFiles*sizeof(int*));
    for (int i=0; i<numberOfFiles; i++){
        matrix_signatures_couples[i] = calloc(numberOfFiles,sizeof(int));
    }
    //array di coppie doc_id + signature di tutti i documenti analizzati
    struct sign_doc* files_sketches = (struct sign_doc*) malloc(numberOfFiles * N_SIGNATURES * sizeof(struct sign_doc));


    start=omp_get_wtime();
   
        #pragma omp parallel for num_threads(NUMB_OF_THREADS)
            for(int i=0; i < numberOfFiles; i++){
                    get_sketches(i, files_sketches+(i*N_SIGNATURES) , *(minhashDocumenti+i));
            }
        end = omp_get_wtime();
        elapsed = end - start;
        exectimes(elapsed, GET_SKETCHES, SET_TIME);

        start=omp_get_wtime();
        mergesort_s_signatures(files_sketches, 0, N_SIGNATURES*numberOfFiles-1);
        
    //cerca e conta le signatures condivise tra tutti i documenti
    create_signatures_matrix(files_sketches, numberOfFiles, matrix_signatures_couples);
    free(files_sketches);

    float *some_results = calloc(20, sizeof(float));
    //cerca i documenti che condividono signatures e le stampa a schermo
    print_similarities(matrix_signatures_couples, numberOfFiles, files, some_results);

    end = omp_get_wtime();
    exectimes(elapsed, FIND_SIMILARITY, SET_TIME);

    for(int i=0; i<20; i++)
        if(some_results[i]>0)
            printf("%.3f  ", some_results[i]);
//
    printf("\n\n");
    return 0;
}


void create_signatures_matrix(struct sign_doc* files_sketches, int numberOfFiles, int **matrix_couples){
    double start;
    double  end;
    start = omp_get_wtime();

    long tot = numberOfFiles * N_SIGNATURES;        
    long long unsigned signature_curr;
    int signature_succ;
    int x,y,temp;

    #pragma omp parallel for private(signature_curr, signature_succ, x, y, temp) num_threads(NUMB_OF_THREADS)// shared(tot, count) schedule(static, 1)
        for (int i = 0; i < tot; i++) {

            signature_curr = files_sketches[i].signature;
            signature_succ = i + 1;

            //fino a che la signature successiva è uguale alla signature corrente:
            while (signature_succ < tot && files_sketches[signature_succ].signature == signature_curr) {
                x = files_sketches[i].doc_id;
                y = files_sketches[signature_succ].doc_id;
                if (y < x ){                  //viene utilizzata solo la diagonale superiore della matrice delle signatures condivise
                    #pragma omp atomic
                    matrix_couples[y][x]++;   //gli x > y non sono elaborati dagli stessi thread, ci può essere corsa critica
                }
                else
                    matrix_couples[x][y]++;   //gli x <= y sono gestiti tutti dallo stesso thread, non ci sono corse critiche

                signature_succ++;
            }
        }

    end = omp_get_wtime();
    exectimes(end-start, CREATE_TRIPLETS, SET_TIME);
 
}


void print_similarities(int **matrix_signatures_couples, int numberOfFiles, char **files, float *some_results){
    float similarity;
    int n=0;

    #pragma omp parallel for private(similarity) shared(n) num_threads(NUMB_OF_THREADS)
        for (int i=0; i<numberOfFiles; i++){
            for(int j=i+1; j<numberOfFiles; j++){

                if( i!=j && matrix_signatures_couples[i][j]>5){
                    similarity = (float)matrix_signatures_couples[i][j]/200;
                    printf("%s\n%s\n condividono: %d signature(s)\n similarità: %.3f\n\n", files[i], files[j], matrix_signatures_couples[i][j], similarity);
                    
                    #pragma omp critical      //questo serve solo per stampare alcuni risultati, non è stato utilizzato per misurare i tempi
                    {
                        if(similarity>0.5  && n<20)
                        some_results[n++]=similarity;
                    }
                }
            }
        }
}





void merge_signatures(struct sign_doc* X, int l, int m, int r) {

    struct sign_doc temp[m-l+1], temp2[r-m];
    //copia la prima parte in un array temporaneo
    for(int i=0; i<(m-l+1); i++)
        temp[i]=X[l+i];
    //copia la seconda parte in un array temporaneo
    for(int i=0; i<(r-m); i++)
        temp2[i]=X[m+1+i];
    int i=0, j=0, k=l;
    //combina i due array
    while(i<(m-l+1) && j<(r-m))
    {
        if(temp[i].signature<temp2[j].signature)
            X[k++]=temp[i++];
        else
            X[k++]=temp2[j++];
    }
    //combina il resto dei due array
    while(i<(m-l+1))
        X[k++]=temp[i++];
    while(j<(r-m))
        X[k++]=temp2[j++];
}



void mergesort_s_signatures(struct sign_doc* X, int l, int n)
{

    if (l < n) {
        int m=(l+n)/2;
        #pragma omp parallel sections num_threads(2)
        {
            #pragma omp section
            {
               mergesort_s_signatures_Serial(X, l, m);
            }

            #pragma omp section
            {
               mergesort_s_signatures_Serial(X, m+1, n);
            }
        }
        merge_signatures(X, l, m, n);
    }
}

void mergesort_s_signatures_Serial(struct sign_doc* X, int l, int n)
{

    if (l < n) {
        int m=(l+n)/2;
        mergesort_s_signatures_Serial(X, l, m);
        mergesort_s_signatures_Serial(X, m+1, n);
        merge_signatures(X, l, m, n);
    }
}

