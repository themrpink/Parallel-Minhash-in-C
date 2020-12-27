#include <omp.h>
#include "shingle_extract_omp.h"
#include "time_test.h"

char ** get_shingles(FILE *fp,  long *tot_shingles, int thread_count){

    double start;
    double end;

    //dimensione del file
    fseek(fp, 0, SEEK_END); // seek to end of file
    long size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET); // seek back to beginning of file


    //numero di shingles
    *tot_shingles = size - K_SHINGLE + 1;


    //alloca lo spazio per gli shingles
    char **shingles = (char **) malloc((*tot_shingles) * (sizeof(char*)));

    #pragma omp parallel for num_threads(thread_count)
        for(int i=0; i<*tot_shingles; i++)
            shingles[i] = (char *)malloc(K_SHINGLE*(sizeof(char)));


    //estrare gli shingles in version parallela
    start = omp_get_wtime();
    shingle_extract_buf_r(fp, size, shingles, thread_count);
    end = omp_get_wtime();
    exectimes(end - start, Parallel_shingles,0);
    return shingles;

}


int shingle_extract_buf_r(FILE * fp, const long filesize, char **shingles, int thread_count){

    char *buffer = (char*)malloc( filesize * sizeof(char));
    fread(buffer, 1, filesize, fp);

    long numb_shingles = filesize - K_SHINGLE +1;

    #pragma omp parallel for num_threads(thread_count) //collapse(2)//schedule(static,c)  //
        for(int count=0; count<numb_shingles; count++){
            for(int pos=0; pos<K_SHINGLE; pos++)    
                shingles[count][pos]=buffer[count+pos];         
        }
   
   //print_shingles(numb_shingles, shingles);
    return 0;

}



char ** get_shingles_serial(FILE *fp,  long *tot_shingles, int thread_count){

    double start;
    double end;

    //dimensione del file
    fseek(fp, 0, SEEK_END); // seek to end of file
    long size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET); // seek back to beginning of file


    //numero di shingles
    *tot_shingles = size - K_SHINGLE + 1;


    //alloca lo spazio per gli shingles
    char **shingles = (char **) malloc((*tot_shingles) * (sizeof(char*)));

    //#pragma omp parallel for num_threads(thread_count)
    for(int i=0; i<*tot_shingles; i++)
        shingles[i] = (char *)malloc(K_SHINGLE*(sizeof(char)));


    //estrare gli shingles in version seriale
    start = omp_get_wtime();
    shingle_extract_buf(fp, size, shingles);
    end = omp_get_wtime();
    exectimes(end - start, Serial_shingles,0);
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

/*
long shingle_extract_buf_hashmap(FILE * fp, const long filesize, char **shingles, int thread_count){


    long const tot_shingles =filesize - K_SHINGLE +1;
    long buckets = tot_shingles / 23;

    

    int count_disc =0;
    long count_c = 0;

    char ***map = (char***) calloc(buckets,sizeof(unsigned long long*));
    int map_count[buckets];
    char **set_signatures_discard = (char**) calloc(tot_shingles, sizeof(char*));

    for(int i=0; i<tot_shingles ;i++)
        *(set_signatures_discard+i) = (char*) calloc(K_SHINGLE+1, sizeof(char));
    

    #pragma omp parallel num_threads(thread_count)
    {
        int rank = omp_get_thread_num();
        int b = buckets / thread_count;
        int start = b * rank;
        int end = b * (rank +1);
        if (rank==thread_count-1)
            end = buckets;
        for(int i=start; i<end;i++){
            map_count[i]=0;
            map[i]= (char**) calloc(50,sizeof(char**));
        }
    }

    
    #pragma omp parallel for num_threads(thread_count) collapse(2)
        
        for(int i=0; i<buckets;i++){
           for(int j=0; j<50; j++){ 
                map[i][j]= (char*) malloc((K_SHINGLE+1)*sizeof(char));
                map[i][j][K_SHINGLE]='\0';
            }
        }


    
    //copia il contenuto del file in una buffer
    char *buffer = (char*)malloc( filesize * sizeof(char));
    fread(buffer, 1, filesize, fp);


    //li estrae e salva in **shingles
    #pragma omp parallel num_threads(thread_count) shared(count_c,count_disc, map, map_count)
    {

        int rank = omp_get_thread_num();
        int b = tot_shingles / thread_count;
        int start = b * rank;
        int end = b * (rank+1);
        if (rank==thread_count-1)
            end = tot_shingles;

        unsigned long long hash_value;
        unsigned long long temp_hash;
        unsigned int moded;

        int check=1;
        int elmt_in_bkt;
        char shingle_str[K_SHINGLE+1];
        char shingle[K_SHINGLE];
        shingle_str[K_SHINGLE]='\0';

        for(int count=start; count<end; count++){

            for(int pos=0; pos<K_SHINGLE; pos++){
                shingle_str[pos] = buffer[count+pos];
                shingle[pos] = buffer[count+pos];
            }
              
 
            for(int i=1; i<K_SHINGLE; i+=2){
                temp_hash=shingle[i-1] * shingle[i];
                hash_value+=temp_hash;
            }

            moded = hash_value % buckets;
            hash_value=0;

            elmt_in_bkt = map_count[moded];

            #pragma omp critical
            {
                for (int i=0; i<elmt_in_bkt; i++){
                    if(strcmp(map[moded][i], shingle_str)==0){
                        strcpy(*(set_signatures_discard+count_disc), shingle_str);
                        count_disc++;
                        check = 0;
                        break;
                    }
                }
        
                if (check == 1){
                    map_count[moded]+=1;
                    elmt_in_bkt = map_count[moded];
                    strcpy(*(shingles+count_c),shingle);
                    strcpy(map[moded][elmt_in_bkt],shingle_str); 
                    count_c++;
                }
            }
            check = 1;

        }
    }

 //   printf("discarded: %d\n", count_disc);

   return tot_shingles;

}


*/


/*
char ** get_shingles_hashmap(FILE *fp,  long *tot_shingles, int thread_count){

    double start;
    double end;

    //dimensione del file
    fseek(fp, 0, SEEK_END); // seek to end of file
    long size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET); // seek back to beginning of file


    //numero di shingles
    *tot_shingles = size - K_SHINGLE + 1;


    //alloca lo spazio per gli shingles
    char **shingles = (char **) malloc((*tot_shingles) * (sizeof(char*)));

    //#pragma omp parallel for num_threads(thread_count)
    for(int i=0; i<*tot_shingles; i++)
        shingles[i] = (char *)malloc(K_SHINGLE*(sizeof(char)));


    //estrare gli shingles in version seriale
    fseek(fp, 0, SEEK_SET);
    start = omp_get_wtime();
    *tot_shingles = shingle_extract_buf_hashmap(fp, size, shingles, thread_count);
    end = omp_get_wtime();
    exectimes(end - start, Serial_shingles,0);
    return shingles;

}

*/