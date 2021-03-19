#include "get_similarities.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "time_test.h"

#define N_SIGNATURES 200

int get_sketches(int i, struct sign_doc *file_sketches, long long unsigned *signatures, char *filename){
    double start;
    double  end;

    start=omp_get_wtime();
    for(int s=0; s<N_SIGNATURES; s++){
        file_sketches[s].doc_id = i;
        file_sketches[s].signature = *(signatures+s);

    }
    end = omp_get_wtime();
    exectimes(end-start, GET_SKETCHES, SET_TIME);
    return 0;
}



int find_similarity(int numberOfFiles, char **files, long long unsigned **minhashDocumenti){
    double start;
    double  end;
    double elapsed;

    struct doc_couple* couples = (struct doc_couple*) malloc( ((numberOfFiles *(numberOfFiles+1))/2)* 200*sizeof(struct doc_couple));
    struct sign_doc* files_sketches = (struct sign_doc*) malloc(numberOfFiles * N_SIGNATURES * sizeof(struct sign_doc));

    start=omp_get_wtime();
    for(int i=0; i < numberOfFiles; i++)
        get_sketches(i, files_sketches+(i*N_SIGNATURES) , *(minhashDocumenti+i), files[i]);
    end = omp_get_wtime();
    elapsed = end - start;

    start=omp_get_wtime();
    mergesort_s_signatures(files_sketches, 0, N_SIGNATURES*numberOfFiles-1);
    end = omp_get_wtime();
    exectimes(elapsed, FIND_SIMILARITY, SET_TIME);

    //crea le triple {doc1, doc2, shared_signatures}
    int count = create_triplets(files_sketches, numberOfFiles, couples);

    //ordina per doc_id
    mergesort_s_doc_id(couples,0, count-1);

    //raccogli le coppie di documenti che hanno almeno una signature in comune
    int index = do_clustering(couples, count);


    check_and_print_similarity(minhashDocumenti, couples, index, files);
    return 0;
}


int create_triplets(struct sign_doc* files_sketches, int numberOfFiles, struct doc_couple* couples){
    double start;
    double  end;
    start = omp_get_wtime();

    int count = 0;
    long tot = numberOfFiles * N_SIGNATURES;
    long long unsigned signature_curr;

    for (int i = 0; i < tot; i++) {

        signature_curr = files_sketches[i].signature;
        int signature_succ = i + 1;

            //fino a che la signature successiva è uguale alla signature corrente:
        while (signature_succ < tot && files_sketches[signature_succ].signature == signature_curr) {
                //inserisci in couples una tripla che contiene una coppia di doc_id ed un "1" a indicare che questi condividono una signature
            couples[count].doc_id = files_sketches[i].doc_id;
            couples[count].doc2_id = files_sketches[signature_succ].doc_id;
            couples[count].shared_signatures = 1;
            signature_succ++;

                //questo count servirà più avanti per sommare le signature condivise
            count++;
        }
    }

    end = omp_get_wtime();
    exectimes(end-start, CREATE_TRIPLETS, SET_TIME);
    /*
        ridimensiona l'array con ai primi "count" elementi (tutti quelli cioè che hanno shared_signatures=1)
    */
    void * a = realloc( couples, (count) * sizeof(struct doc_couple));

    free(files_sketches);
    return count;
}




int do_clustering(struct doc_couple* couples, int count) {
    double start;
    double end;

    int index = 0;
    start = omp_get_wtime();
    for (int i = 0; i < count; i++) {
        if (couples[i].shared_signatures != 0) {
            for (int j = i + 1; j < count; j++) {
                if (couples[i].doc_id == couples[j].doc_id && couples[i].doc2_id == couples[j].doc2_id) {
                    couples[i].shared_signatures++;
                    couples[j].shared_signatures = 0;
                }
            }
            couples[index] = couples[i];
            index++;
        }
    }

    end = omp_get_wtime();
    exectimes(end-start, DO_CLUSTERING, SET_TIME);
    void* a = realloc( couples, (index) * sizeof(struct doc_couple));
    return index;
}



void merge_doc_id(struct doc_couple* X, int l, int m, int r) {
    struct doc_couple temp[m-l+1], temp2[r-m];
    //copy first part to temporary array
    for(int i=0; i<(m-l+1); i++)
        temp[i]=X[l+i];
    //copy second part to temporary array
    for(int i=0; i<(r-m); i++)
        temp2[i]=X[m+1+i];
    int i=0, j=0, k=l;
    //combine the arrays on the basis of order
    while(i<(m-l+1) && j<(r-m))
    {
        if(temp[i].doc_id<temp2[j].doc_id)
            X[k++]=temp[i++];
        else
            X[k++]=temp2[j++];
    }
    //to combine the remainder of the two arrays
    while(i<(m-l+1))
        X[k++]=temp[i++];
    while(j<(r-m))
        X[k++]=temp2[j++];
}


void merge_signatures(struct sign_doc* X, int l, int m, int r) {

    struct sign_doc temp[m-l+1], temp2[r-m];
    //copy first part to temporary array
    for(int i=0; i<(m-l+1); i++)
        temp[i]=X[l+i];
    //copy second part to temporary array
    for(int i=0; i<(r-m); i++)
        temp2[i]=X[m+1+i];
    int i=0, j=0, k=l;
    //combine the arrays on the basis of order
    while(i<(m-l+1) && j<(r-m))
    {
        if(temp[i].signature<temp2[j].signature)
            X[k++]=temp[i++];
        else
            X[k++]=temp2[j++];
    }
    //to combine the remainder of the two arrays
    while(i<(m-l+1))
        X[k++]=temp[i++];
    while(j<(r-m))
        X[k++]=temp2[j++];
}



void mergesort_s_signatures(struct sign_doc* X, int l, int n)
{
    if (l < n) {
        int m=(l+n)/2;
        mergesort_s_signatures(X, l, m);
        mergesort_s_signatures(X, m+1, n);
        merge_signatures(X, l, m, n);
    }
}

void mergesort_s_doc_id(struct doc_couple*  X, int l, int n)
{
    if (l < n) {
        int m=(l+n)/2;
        mergesort_s_doc_id(X, l, m);
        mergesort_s_doc_id(X, m+1, n);
        merge_doc_id(X, l, m, n);
    }
}


void check_and_print_similarity(long long unsigned **minhashDocumenti, struct doc_couple* couples, int index, char **files){
    double some_results[20] = {0};
    int doc1;
    int doc2;
    int j=0;
    int count_files = 0;
    printf("\n\n");
    for(int i=0; i<index;i++){
        doc1 = couples[i].doc_id;
        doc2 = couples[i].doc2_id;

        if(couples[i].shared_signatures > 5){
            printf("%s\n%s\n condividono: %d signature(s)\n", files[doc1], files[doc2], couples[i].shared_signatures);
            printf("similarità: %.3f\n\n",(float)couples[i].shared_signatures/N_SIGNATURES);
        }
        if(((float)couples[i].shared_signatures/N_SIGNATURES)>0.5 && j<20){
            some_results[j++] =(float)couples[i].shared_signatures/N_SIGNATURES;

            count_files++;
        }
            
    }

    FILE *fp = fopen("similarity_results.txt", "a");
    printf("--> alcuni risultati di somiglianza tra files: (salvati anche in \"similarity_results.txt\")\n");
    int c = 0;
    if(j>20)
        j=20;

    for(int i=0; i<j; i++)
        if(some_results[i]>0){
            printf("%.3f  ", some_results[i]);
            fprintf(fp, "%.3f   ", some_results[i]);
        }
    if(count_files==0)
        printf("    nessuna somiglianza tra i files");
    printf("\n\n");
    fprintf(fp, "\n");
    fclose(fp);

}