#include "shingle_extract_omp.h"





int shingle_extract_buf(FILE * fp, const long filesize, int thread_count, char **shingles){

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






int shingle_extract_buf_r(FILE * fp, const long filesize, int thread_count, char **shingles){

    char *buffer = (char*)malloc( filesize * sizeof(char));
    fread(buffer, 1, filesize, fp);

    long numb_shingles = filesize - K_SHINGLE +1;

    #pragma omp parallel for num_threads(thread_count)

        for(int count=0; count<numb_shingles; count++){
    
      //  #pragma omp parallel for num_threads(2)
            for(int pos=0; pos<K_SHINGLE; pos++)
                
                shingles[count][pos]=buffer[count+pos];         
        }
   
   //print_shingles(numb_shingles, shingles);
    return 0;

}




void print_shingles(int tot_shingles, char **shingles){

        //stampa gli shingles estratti dal documento

    for(int i=0; i<tot_shingles-1; i++){

        for(int j=0; j<K_SHINGLE; j++)
            printf("%c", shingles[i][j]);
        
        printf("\n");
    }
}



/*
//questa è pensata per pthread, work in progress
int shingle_extract_pthread(char *buffer, const long filesize, char **shingles){

    int rank=0;         //lo ricavo in qualche modo
    int tot_threads=1;    //lo ricavo dalla dimensione del file diviso una dimensione definita globalmente : filesize / BUF
    int count=0;
  //  int numb_shingles = filesize - (K_SHINGLE) + 1;  //numero tot di shingles
    int first_letter = rank * BUF; //questo è l'indice del carattere del buffer
    int index_shingle = BUF*rank;  //questo è l'indice del primo shingle
    char letter;
    int c=0;
    int i;
    int l;

    //come omp si può provare a dividere ulteriormente questo for
    for (l = 0 ; l< 2*BUF; l++){
        letter = buffer[first_letter + l];
        if (count<K_SHINGLE && rank==0){
            c=count;
            for (i=0; i<=c; i++)
                shingles[index_shingle + i][c-i] = letter;
        }
        else{
            
            //si può provare a assegnare ogni for a un thread? come fare? cioè ogni ciclo completo di for a un thread. forse no
            for (i=0; i<K_SHINGLE; i++)
                shingles[index_shingle + count - K_SHINGLE+ i][K_SHINGLE-1-i] = letter;
        }
        count++;
    }
    //per estrarre le ultime lettere
    if (rank==tot_threads-1){
        c=0;
        for ( ; l < filesize; l++){
            letter = buffer[first_letter + l];
            for (i=c; i<K_SHINGLE; i++)
                shingles[index_shingle + count +i][K_SHINGLE-i+c] = letter;
            c++;
        }
    }
//incrementado la c, finisco con c=k-1, quindi i = k-1 e quindi inserisco l'ultima lettera nell'ultima posizione (k-1) dell'ultimo


   // print_shingles(numb_shingles, shingles);

    return 0;
}

*/

