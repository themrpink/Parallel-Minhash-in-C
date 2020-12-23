
#include "shingle_extract.h"



char ** get_shingles(FILE *fp,  long *tot_shingles){

    //dimensione del file
    fseek(fp, 0, SEEK_END); // seek to end of file
    long size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET); // seek back to beginning of file


    //numero di shingles
    *tot_shingles = size - K_SHINGLE + 1;


    //alloca lo spazio per gli shingles
    char **shingles = (char **) malloc((*tot_shingles) * (sizeof(char*)));
    for(int i=0; i<*tot_shingles; i++)
        shingles[i] = (char *)malloc(K_SHINGLE*(sizeof(char)));
    

    //estrae gli shingles
    shingle_extract_buf(fp, size, shingles);


    return shingles;

}




int shingle_extract_buf(FILE * fp, const long filesize, char **shingles){

    //copia il contenuto del file in una buffer
    char *buffer = (char*)malloc( filesize * sizeof(char));
    fread(buffer, 1, filesize, fp);



    //ricava il numero di shingles
    long numb_shingles = filesize - K_SHINGLE +1;

    //li estrae e salva in **shingles
    for(int count=0; count<numb_shingles; count++){

        for(int pos=0; pos<K_SHINGLE; pos++)
            shingles[count][pos]=buffer[count+pos];   
    }

  // print_shingles(numb_shingles, shingles);
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



