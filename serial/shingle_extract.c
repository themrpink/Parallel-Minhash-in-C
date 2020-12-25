
#include "shingle_extract.h"





void shingle_extract_buf(char* buf, long numb_shingles, char **shingles){

    for(long long i=0; i<numb_shingles; i++)
        shingles[i] = (char *)malloc(K_SHINGLE*(sizeof(char)));
    //li estrae e salva in **shingles
    for(int count=0; count<numb_shingles; count++){

        for(int pos=0; pos<K_SHINGLE; pos++)
            shingles[count][pos]=buf[count+pos];   
    }
}





void print_shingles(long long tot_shingles, char **shingles){

        //stampa gli shingles estratti dal documento
    printf("\n############## shingles: ##############\n\n");
    for(long long i=0; i<tot_shingles-1; i++){

        for(int j=0; j<K_SHINGLE; j++){
            printf("%c", shingles[i][j]);
        }
        printf("\n");
    }
}



