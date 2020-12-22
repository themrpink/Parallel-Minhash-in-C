
#include "shingle_extract.h"

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

        if(fread(&c, 1, 1, fp) == -1)
            return -1;
            
       // pos=K_SHINGLE-1;

        for (int i = 0; i < K_SHINGLE; i++){
            shingles[count+i][K_SHINGLE-1-i] = c;
        }
    }


    int l = K_SHINGLE-1;

    //completa gli ultimi shingles
    for( ; count<numb_shingles-1; count++){

      //  pos=K_SHINGLE-1;
        if(fread(&c, 1, 1, fp)==-1)
            return -1;

        for(int k=0 ; k<l; k++)
            shingles[count+k][K_SHINGLE-1-k] = c;
        
        l--;
    }

   // print_shingles(numb_shingles, shingles);

    return 0;
}





int shingle_extract_buf(FILE * fp, const long filesize, char **shingles){

    char *buffer = (char*)malloc( filesize * sizeof(char));
    fread(buffer, 1, filesize, fp);

    long numb_shingles = filesize - K_SHINGLE +1;

    int pos=0;
    int count;


    for(count=0; count<numb_shingles; count++){

        for(pos=0; pos<K_SHINGLE; pos++)
            
            shingles[count][pos]=buffer[count+pos];
        
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
