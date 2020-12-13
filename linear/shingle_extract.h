
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>

#define PRIMES_SIZE 200
#define K_SHINGLE 20
#define BANDS 20

void print_shingles(int tot_shingles, char **shingles);
int shingle_extract(FILE * fp, const long numb_shingles, char **shingles);

int shingle_extract(FILE * fp, const long numb_shingles, char **shingles){

    char c;
    int pos=0;
    int count;

    //estrae i primi K_SHINGLE caratteri e li metti nei primi shingles
    for(count=0; count<K_SHINGLE; count++){

        fread(&c, 1, 1, fp);
        pos=count;

        for(int k=0; k<count+1; k++){
            shingles[k][pos--] = c;
        }
    }

    //crea il corpo centrale degli shingles
    for(count=1; count<numb_shingles-K_SHINGLE; count++){

        if(fread(&c, 1, 1, fp)==-1)
            return -1;
            
        pos=K_SHINGLE-1;

        for (int i =0; i<K_SHINGLE; i++){
            shingles[count+i][pos--]=c;
        }
    }


    int l = K_SHINGLE-1;

    //completa gli ultimi shingles
    for(count; count<numb_shingles-1; count++){

        pos=K_SHINGLE-1;
        if(fread(&c, 1, 1, fp)==-1)
            return -1;

        for(int k=0 ; k<l; k++)
            shingles[count+k][pos--] = c;
        
        l--;
    }

    print_shingles(numb_shingles, shingles);

    return 0;
}



void print_shingles(int tot_shingles, char **shingles){

        //stampa gli shingles estratti dal documento

    printf("\n############## shingles: ##############\n\n");
    for(int i=0; i<tot_shingles-1; i++){

        for(int j=0; j<K_SHINGLE; j++){
            printf("%c", shingles[i][j]);
        }
        printf("\n");
    }
}