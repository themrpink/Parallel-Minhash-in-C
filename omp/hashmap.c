

    long buckets = tot_shingles / 29;
   // int buckets = 200;
    unsigned int moded;
//    char set_signatures[tot_shingles][K_SHINGLE+1]; //= calloc(tot_shingles, sizeof(unsigned long long*));
    char ***map = (char***) calloc(buckets,sizeof(unsigned long long*));
    int map_count[buckets];
    char **set_signatures_discard = (char**) calloc(tot_shingles, sizeof(char*));
    char **set_signatures = (char**) calloc(tot_shingles, sizeof(char*));
    for(int i=0; i<tot_shingles ;i++){
        *(set_signatures+i) = (char*) calloc(K_SHINGLE+1, sizeof(char));
        *(set_signatures_discard+i) = (char*) calloc(K_SHINGLE+1, sizeof(char));
    }

    for(int i=0; i<buckets;i++){
        map_count[i]=0;
        map[i]= (char**) calloc(50,sizeof(char**));
        for(int j=0; j<50; j++){
            map[i][j]= (char*) malloc((K_SHINGLE+1)*sizeof(char));
            map[i][j][K_SHINGLE]='\0';
        }
    }
    //unsigned long long set_signatures[200];
    //unsigned long long map[buckets][10];
    /*
    for(int i=0; i<200;i++){
        for (int j=0; j<10;j++)
            map[i][j]=0;
    }
   
    int check = 1;
    long count = 0;
    int count_disc = 0;
    unsigned long long hash_value;
    unsigned long long temp_hash;
    printf("buck: %ld\n", buckets);
    int c;


    start = omp_get_wtime();
    for (long l = 0; l < tot_shingles; l++){
        hash_value=0;
        for(int i=1; i<K_SHINGLE; i+=2){
            temp_hash=shingles[l][i-1] * shingles[l][i];
            hash_value+=temp_hash;
            
            //printf(" hash: %llu\n", hash_value);
        }
        //hash_value=hash_value/29;
        moded = hash_value % buckets;
      //  printf("buck: %d , sh: %s, hash: %llu\n", moded, *(shingles+l), hash_value);
        c = map_count[moded];

        for (int i=0; i<c; i++)
            if(strcmp(map[moded][i], shingles[l])==0){
                *(set_signatures_discard+count_disc) = shingles[l];
                count_disc++;
                check = 0;
                break;
            }
                

        if (check == 1){
            map_count[moded]+=1;
            c = map_count[moded];
            //printf("%d\n",c);
            map[moded][c] = shingles[l];
            set_signatures[count] = shingles[l];
            count+=1;
        }

        check = 1;
    }
/*
    for(int i = 0; i<count; i++){
        printf("%d: %s\n", i, set_signatures[i]);
    }


    end = omp_get_wtime();
    printf("\n \n time: %f\n\n", end - start);

    int max=0;
    int cc=0;
    for(int i = 0; i<buckets; i++){
        if(map_count[i]==0)
            cc++;
        //if(map_count[i]>max)
            max=map_count[i];
        //printf("bucks: %d: %d\n", i, map_count[i]);
    }

 */
 /*
    for(int i = 0; i<=count_disc; i++){

        printf("%d: %s\n", i, set_signatures_discard[i]);
    }

    printf("tot shingles: %ld\n", tot_shingles);
    printf("tot bucks: %ld\n", buckets);
    printf("empy bucks: %d\n", cc);
    printf("max bucks: %d\n", max);
    printf("discarded: %d\n", count_disc);*/