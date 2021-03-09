#include "get_similarities.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "time_test.h"
#include <pthread.h>

#define N_SIGNATURES 200
#define N_THREADS 4

pthread_mutex_t lock;
pthread_mutex_t lock_count_shared_signature;

typedef struct {
    int index_inizio;
    int index_fine;
    struct sign_doc* files_sketches;
} args_signatures ;

typedef struct {
    int rank;
    struct sign_doc* files_sketches;
    int numberOfFiles;
    struct doc_couple* couples;
    int* count;
} args_create_triplets ;

typedef struct {
    int index_inizio;
    int index_fine;
    struct doc_couple* couples;
} args_mergesort_docid ;

typedef struct {
    int rank;
    struct doc_couple* couples;
    int count;
} args_count_shared_signature ;

int get_sketches(int i, struct sign_doc *file_sketches, long long unsigned *signatures){

    for(int s=0; s<N_SIGNATURES; s++){
        file_sketches[s].doc_id = i;
        file_sketches[s].signature = *(signatures+s);
    }
    return 0;
}


int find_similarity(int numberOfFiles, char **files, long long unsigned **minhashDocumenti){
    pthread_t threads[N_THREADS];
    struct doc_couple* couples = (struct doc_couple*) malloc( ((numberOfFiles *(numberOfFiles+1))/2)* N_SIGNATURES*sizeof(struct doc_couple));
    struct sign_doc* files_sketches = (struct sign_doc*) malloc(numberOfFiles * N_SIGNATURES * sizeof(struct sign_doc));
    long files_sketchs_length= N_SIGNATURES*numberOfFiles-1;
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);
    //region get_sketches
    for(int i=0; i < numberOfFiles; i++)
        get_sketches(i, files_sketches+(i*N_SIGNATURES) ,*(minhashDocumenti+i));
    //endregion
    exectimes(getElapsedTime(&begin, &end), GET_SKETCHES, SET_TIME);
    //region merge_signature

    clock_gettime(CLOCK_REALTIME, &begin);


    args_signatures args[N_THREADS];

    for (int i = 0; i < N_THREADS; ++i) {
        args[i].index_inizio=i*files_sketchs_length/N_THREADS;
        if ((i+1)==N_THREADS){
            int resto=files_sketchs_length%N_THREADS;
            args[i].index_fine=((i+1)*files_sketchs_length/N_THREADS)+resto;
        }else{
            args[i].index_fine=((i+1)*files_sketchs_length/N_THREADS);
        }
        args[i].files_sketches=files_sketches;
    }
    int j=0;
    while(j<N_THREADS){
        int thread_created=pthread_create(&threads[j],NULL,mergesort_signatures,(void*)&args[j]);
        if (thread_created==0){
            j++;
        }
    }

    for (int i = 0; i < N_THREADS; ++i) {
        pthread_join(threads[i],NULL);
    }
    merge_signatures(files_sketches,args[0].index_fine, args[0].index_fine, args[1].index_fine);
    merge_signatures(files_sketches,args[2].index_inizio, args[2].index_fine,args[3].index_fine);
    merge_signatures(files_sketches,args[0].index_inizio, args[1].index_fine, args[3].index_fine);


    //endregion
    exectimes(getElapsedTime(&begin, &end), MERGE_SORT, SET_TIME);
    //region create_triplets
    clock_gettime(CLOCK_REALTIME, &begin);
    args_create_triplets args_triplets[N_THREADS];
    pthread_mutex_init(&lock, NULL);
    int count_triplets=0;

    for (int i = 0; i < N_THREADS; ++i) {
        args_triplets[i].rank=i;
        args_triplets[i].files_sketches=files_sketches;
        args_triplets[i].numberOfFiles=numberOfFiles;
        args_triplets[i].couples=couples;
        args_triplets[i].count=&count_triplets;
    }

    while(j<N_THREADS){
        int thread_created=pthread_create(&threads[j],NULL,create_triplets,(void*)&args_triplets[j]);
        if (thread_created==0){
            j++;
        }
    }
    for (int i = 0; i < N_THREADS; ++i) {
        pthread_join(threads[i],NULL);
    }
    /*
    ridimensiona l'array con ai primi "count" elementi (tutti quelli cioè che hanno shared_signatures=1)
    */
    realloc(couples, (--count_triplets) * sizeof(struct doc_couple));
    pthread_mutex_destroy(&lock);
    //endregion
    exectimes(getElapsedTime(&begin, &end), CREATE_TRIPLETS, SET_TIME);
    free(files_sketches);
    //region merge sort doc id
    args_mergesort_docid args_merge_sort[N_THREADS];
    for (int i = 0; i < N_THREADS; ++i) {
        args_merge_sort[i].index_inizio=i*count_triplets/N_THREADS;
        if ((i+1)==N_THREADS){
            args_merge_sort[i].index_fine=count_triplets-1;
        }else{
            args_merge_sort[i].index_fine=((i+1)*count_triplets/N_THREADS);
        }
        args_merge_sort[i].couples=couples;
    }
    j=0;
    while(j<N_THREADS){
        int thread_created=pthread_create(&threads[j],NULL,mergesort_doc_id,(void*)&args[j]);
        if (thread_created==0){
            j++;
        }
    }

    for (int i = 0; i < N_THREADS; ++i) {
        pthread_join(threads[i],NULL);
    }


    merge_doc_id(files_sketches,args_merge_sort[0].index_fine, args_merge_sort[0].index_fine, args_merge_sort[1].index_fine);
    merge_doc_id(files_sketches,args_merge_sort[2].index_inizio, args_merge_sort[2].index_fine,args_merge_sort[3].index_fine);
    merge_doc_id(files_sketches,args_merge_sort[0].index_inizio, args_merge_sort[1].index_fine, args_merge_sort[3].index_fine);
    //endregion
    //raccogli le coppie di documenti che hanno almeno una signature in comune
    int index = do_clustering(couples, count_triplets);


    check_and_print_similarity(minhashDocumenti, couples, index, files);
    return 0;
}

/**
 * crea le triple {doc1, doc2, shared_signatures}
 * @param files_sketches
 * @param numberOfFiles
 * @param couples
 * @return
 */
void* create_triplets(void* args){

    long long unsigned signature_curr;
    long numThread=((args_create_triplets*)args)->rank;
    long tot = ((args_create_triplets*)args)->numberOfFiles * N_SIGNATURES;
    long firstRow=numThread*tot/N_THREADS;

    long lastRow;
    if ((numThread+1)==N_THREADS){
        lastRow=tot;
    }else{
        lastRow=(numThread+1)*tot/N_THREADS;
    }
    for (int i = firstRow; i < lastRow; i++) {

        signature_curr = ((args_create_triplets*)args)->files_sketches[i].signature;
        int signature_succ = i + 1;

        //fino a che la signature successiva è uguale alla signature corrente:
        while (signature_succ < tot && ((args_create_triplets*)args)->files_sketches[signature_succ].signature == signature_curr) {
            //inserisci in couples una tripla che contiene una coppia di doc_id ed un "1" a indicare che questi condividono una signature
            pthread_mutex_lock(&lock);
            ((args_create_triplets*)args)->couples[*((args_create_triplets*)args)->count].doc_id = ((args_create_triplets*)args)->files_sketches[i].doc_id;
            ((args_create_triplets*)args)->couples[*((args_create_triplets*)args)->count].doc2_id = ((args_create_triplets*)args)->files_sketches[signature_succ].doc_id;
            ((args_create_triplets*)args)->couples[*((args_create_triplets*)args)->count].shared_signatures = 1;
            signature_succ++;
            *((args_create_triplets*)args)->count+=1;
            pthread_mutex_unlock(&lock);

        }
    }
}



void* mergesort_signatures(void* args){
    mergesort_s_signatures(((args_signatures*)args)->files_sketches,((args_signatures*)args)->index_inizio,((args_signatures*)args)->index_fine) ;
}

void merge_signatures(struct sign_doc* X, int index_inizio, int valore_medio, int index_fine) {

    struct sign_doc temp[valore_medio - index_inizio + 1], temp2[index_fine - valore_medio];
    //copy first part to temporary array
    for(int i=0; i<(valore_medio - index_inizio + 1); i++)
        temp[i]=X[index_inizio + i];
    //copy second part to temporary array
    for(int i=0; i<(index_fine - valore_medio); i++)
        temp2[i]=X[valore_medio + 1 + i];
    int i=0, j=0, k=index_inizio;
    //combine the arrays on the basis of order
    while(i<(valore_medio - index_inizio + 1) && j < (index_fine - valore_medio))
    {
        if(temp[i].signature<temp2[j].signature)
            X[k++]=temp[i++];
        else
            X[k++]=temp2[j++];
    }
    //to combine the remainder of the two arrays
    while(i<(valore_medio - index_inizio + 1))
        X[k++]=temp[i++];
    while(j<(index_fine - valore_medio))
        X[k++]=temp2[j++];
}

void mergesort_s_signatures(struct sign_doc* X, int index_inizio, int index_fine)
{
    if (index_inizio < index_fine) {
        int m= (index_inizio + index_fine) / 2;
        mergesort_s_signatures(X, index_inizio, m);
        mergesort_s_signatures(X, m+1,index_fine);
        merge_signatures(X, index_inizio, m, index_fine);
    }
}

void* mergesort_doc_id(void* args){
    mergesort_s_doc_id(((args_mergesort_docid*)args)->couples,((args_mergesort_docid*)args)->index_inizio,((args_mergesort_docid*)args)->index_fine);
}

void merge_doc_id(struct doc_couple* X, int index_inizio, int valore_medio, int index_fine) {
    struct doc_couple temp[valore_medio - index_inizio + 1], temp2[index_fine - valore_medio];
    //copy first part to temporary array
    for(int i=0; i<(valore_medio - index_inizio + 1); i++)
        temp[i]=X[index_inizio + i];
    //copy second part to temporary array
    for(int i=0; i<(index_fine - valore_medio); i++)
        temp2[i]=X[valore_medio + 1 + i];
    int i=0, j=0, k=index_inizio;
    //combine the arrays on the basis of order
    while(i<(valore_medio - index_inizio + 1) && j < (index_fine - valore_medio))
    {
        if(temp[i].doc_id<temp2[j].doc_id)
            X[k++]=temp[i++];
        else
            X[k++]=temp2[j++];
    }
    //to combine the remainder of the two arrays
    while(i<(valore_medio - index_inizio + 1))
        X[k++]=temp[i++];
    while(j<(index_fine - valore_medio))
        X[k++]=temp2[j++];
}

void mergesort_s_doc_id(struct doc_couple*  X, int index_inizio, int index_fine)
{
    if (index_inizio < index_fine) {
        int m=(index_inizio+index_fine)/2;
        mergesort_s_doc_id(X, index_inizio, m);
        mergesort_s_doc_id(X, m+1, index_fine);
        merge_doc_id(X, index_inizio, m, index_fine);
    }
}

int do_clustering(struct doc_couple* couples, int count) {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);
    pthread_mutex_init(&lock_count_shared_signature, NULL);
    args_count_shared_signature args_shared_signature[N_THREADS];
    pthread_t threads[N_THREADS];

    for (int i = 0; i < N_THREADS; ++i) {
        args_shared_signature[i].rank=i;
        args_shared_signature[i].couples=couples;
        args_shared_signature[i].count=count;
    }
    int j=0;
    while(j<N_THREADS){
        int thread_created=pthread_create(&threads[j],NULL,count_shared_signature,(void*)&args_shared_signature[j]);
        if (thread_created==0){
            j++;
        }
    }
    for (int i = 0; i < N_THREADS; ++i) {
        pthread_join(threads[i],NULL);
    }
    pthread_mutex_destroy(&lock_count_shared_signature);

    int index = 0;
    for (int i = 0; i < count; i++) {
        if (couples[i].shared_signatures != 0) {
            couples[index] = couples[i];
            index++;
        }
    }


    exectimes(getElapsedTime(&begin, &end), DO_CLUSTERING, SET_TIME);
    realloc( couples, (index) * sizeof(struct doc_couple));
    return index;
}

void* count_shared_signature(void* args) {
    long numThread = ((args_count_shared_signature *) args)->rank;
    long firstRow = numThread * ((args_count_shared_signature *) args)->count / N_THREADS;

    long lastRow;
    if ((numThread + 1) == N_THREADS) {
        lastRow = ((args_count_shared_signature *) args)->count;
    } else {
        lastRow = (numThread + 1) * ((args_count_shared_signature *) args)->count / N_THREADS;
    }

    for (int i = firstRow; i < lastRow; i++) {
        if (((args_count_shared_signature *) args)->couples[i].shared_signatures != 0) {
            int j = i + 1;
            while (((args_count_shared_signature *) args)->couples[i].doc_id ==((args_count_shared_signature *) args)->couples[j].doc_id)
                if (((args_count_shared_signature *) args)->couples[i].doc2_id ==((args_count_shared_signature *) args)->couples[j].doc2_id) {
                    pthread_mutex_lock(&lock_count_shared_signature);
                    ((args_count_shared_signature *) args)->couples[i].shared_signatures += ((args_count_shared_signature *) args)->couples[j].shared_signatures;
                    ((args_count_shared_signature *) args)->couples[j].shared_signatures = 0;
                    pthread_mutex_unlock(&lock_count_shared_signature);
                }
            j++;
        }
    }
}



void check_and_print_similarity(long long unsigned **minhashDocumenti,  struct doc_couple* couples, int index, char **files){
    double some_results[20] = {0};
    int doc1;
    int doc2;
    int shared=0;
    int j=0;
    printf("\n\n");
    for(int i=0; i<index;i++){
        doc1 = couples[i].doc_id;
        doc2 = couples[i].doc2_id;
        for(int signature=0; signature<N_SIGNATURES; signature++)
            if (minhashDocumenti[doc1][signature] == minhashDocumenti[doc2][signature])
                shared+=1;
        printf("%s\n%s\n condividono: %d signature(s)\n", files[doc1], files[doc2], shared);
        printf("similarità: %.3f\n\n",(float)shared/N_SIGNATURES);
        if(((float)shared/N_SIGNATURES)>0.5 && j<20)
            some_results[j++] =(float)shared/N_SIGNATURES;
        shared=0;
    }

    FILE *fp = fopen("similarity_results.txt", "a");
    printf("--> alcuni risultati di somiglianza tra files: (salvati anche in \"similarity_results.txt\")\n");
    int c = 0;
    for(int i=0; i<20; i++)
        if(some_results[i]>0){
            printf("%.3f  ", some_results[i]);
            fprintf(fp, "%.3f   ", some_results[i]);
            c++;
        }
    if(c==0)
        printf("    nessuna somiglianza tra i files");
    printf("\n\n");
    fprintf(fp, "\n");
    fclose(fp);
    free(couples);
}