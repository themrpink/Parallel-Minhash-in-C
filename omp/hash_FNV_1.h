

#define K_SHINGLE 54

unsigned long long primes[] = { 1099511628211,  109951181297,   109951192301,   651571110623,   651571122467, 
                                1000000000039,  1000000001939,  1000000016339,  109951179061,   109951191323,
                                1000000001083,  132951242141,   132951254257,   132951251243,   132951415591, 
                                132951423139,   132991532629,	132991532633,	132991532657,	132991532693,		
                                132991532731,   145991523677,	145991523683,	145991523701,	145991523737,
                                145991535751,	145991535773,	145991535791,	145991535797,   145991614267,	
                                145991614283,	145991614291,	145991614343,   145991620727,   145991620729,		
                                146991637937,	146991637951,	146991637987,	146991637993 ,  146991642143,	
                                146991642151,	146991642191,	146991642229,   216991644997,	216991645009,	
                                216991645049,	216991645133,   216991652231,	216991652281,	216991652291,	
                                216991652299,   216991658771,	216991658783,	216991658789,	216991658831,
                                246971709641,	246971709709,	246971709767,	246971709823 ,  246971722979,	
                                246971722981,	246971722999,	246971723027,   246971723549,	246971723591,		
                                255571756877,   255571760641,	255571760683,	255571760687,	255571760693,	
                                255571770317,	255571770319,	255571770373 ,  291571836389,	291571836401,	
                                291571836427,	291571836529,   291571842491,	291571842509,	291571842533,	
                                291571850239,	291571850261,	291571850327,	291571850347 ,  351571868903,	
                                132991532699,	351571869029,	351571869037,   351571873153,	351571873207,	
                                351571878253,   351571881943,	351571881949,	351571881953,	351571881979,
                                351571882549,	351571882561,	351571882639,	351571882691,   132991532699,
                                411571889473,	411571889507,	411571889527,	411571889557,	411571889569,		
                                411571914487,	411571914521,	411571914529,	411571914569,	411571914577,
                                678923973041,	678923973059,	678923973061,	678923973113,	678923973121,	
                                678923973163,	678923973197,	678923973227,	678923973257,	678923973263,
                                473588577479,	473588577509,	473588577569,	473588577587,	473588577589,	
                                473588577601,	473588577613,	473588577653,	473588577667,	473588577689,
                                948167826623,	948167826631,	948167826761,	948167826763,	948167826769,	
                                948167826773,	948167826779,	948167826883,	948167826913,	948167827031,
                                329868119827,	329868119863,	329868119879,	329868119909,	329868119927,	
                                329868119929,	329868119951,	329868120053,	329868120061,	329868120073,
                                231680873923,	231680873929,	231680873947,	231680873993,	231680873999,	
                                231680874023,	231680874133,	231680874167,	231680874193,	231680874203,
                                255148978289,	255148978291,	255148978307,	255148978313,	255148978367,	
                                255148978459,	255148978471,	255148978493,	255148978507,	255148978517,
                                825918857579,	825918857593,	825918857623,	825918857629,	825918857671,	
                                825918857681,	825918857683,	825918857711,	825918857747,	825918857759,
                                394695996511,	394695996589,	394695996593,	394695996659,	394695996679,	
                                394695996683,	394695996689,	394695996691,	394695996697,	394695996701,
                                416473710067,	416473710071,	416473710073,	416473710077,	416473710097,	
                                416473710127,	416473710157,	416473710223,	416473710269,	416473710281
                            };






/*
    ci sono dipendenze, ogni for loop è dipendente dal precedente.
    se eseguo la stessa funzione invertendo l'ordine dei caratteri cambia 
    il risultato, infatti sarebbe come invertire l'ordine delle lettere hashate.
    Allo stesso modo distribuire il for su thread diversi che non rispettano l'ordine
    di esecuzione corrispono a hashare una permutazione della stringa data.


        questo è il test che dimostra il problema:

        *hash = FNV_offset_basis;
        for(int i=0; i<len; i++){
            (*hash) *= primes[prime_i];
            (*hash) ^= shingle[i];
        }
        printf("hash CRESC: %llu \n", *hash);

        *hash = FNV_offset_basis;
        for(int i=len-1; i>=0; i--){
            (*hash) *= primes[prime_i];
            (*hash) ^= shingle[i];
        }
        printf("hash DECR: %llu \n", *hash);


        quindi questa operazione la possiamo considerare atomica oppure studiare come risolverla
        
*/