
#include "shingle_extract.h"





int shingle_extract_buf(FILE * fp, const long filesize, char **shingles){

    char *buffer = (char*)malloc( filesize * sizeof(char));
    fread(buffer, 1, filesize, fp);

    long numb_shingles = filesize - K_SHINGLE +1;

    for(int count=0; count<numb_shingles; count++){

        for(int pos=0; pos<K_SHINGLE; pos++)
            shingles[count][pos]=buffer[count+pos];   
    }

   //print_shingles(numb_shingles, shingles);
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


void store_shingle(char *docname, char **shingles, long numb_shingles){

    //crea id da nome file con un hash nome file + dimensione + timestamp
    //salva il file con questo id e salva le signatures al suo interno
    //si può anche creare un file XML con tutti gli id dei documenti.
    //come tornare dall'id al documento? posso rinominarlo?

}



//usare un set? 