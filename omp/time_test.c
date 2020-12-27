#include "time_test.h"



void exectimes(double value, enum Tasks task, int print){

    static double results_shingles[] = {0,0,0};
    static double results_signatures[] = {0,0,0};
    static int count1a;
    static int count2a;
    static int count1b;
    static int count2b;

    if(print==0)
        switch (task)
        {
        case Serial_shingles:
            results_shingles[0] += value;
            count1a++;
            break;
        case Serial_signatures:
            results_signatures[0] += value;
            count1b++;
            break;
        case Parallel_shingles:
            results_shingles[1] += value;
            count2a++;
            break;
        case Parallel_signatures:
            results_signatures[1] += value;
            count2b++;
            break;   
        default:
            break;
        } 

    else{
        printf("\n\n########## test versione seriale -- ");
        printf("esecuzioni: %d\n", print);
        printf("\nserial version simple loop:\n f: shingle_extract_buf \n number of threads: %d\n time: %f\n\n", 1 ,  results_shingles[0]/count1a);
        printf("version serial for: \n f: get_signatures_s \n number of threads: %d\n time: %f\n",1, results_signatures[0]/count1b);

        printf("\n\n######## test versione parallela -- ");
        printf("esecuzioni: %d\n", print);
        printf("\nomp version parallel for: \n f: shingle_extract_buf_r \n number of threads: %d\n time: %f\n\n",(int)value, results_shingles[1]/count2a);
        printf("opm version parallel for: \n f: get_signatures \n number of threads: %d\n time: %f\n\n",(int)value,  results_signatures[1]/count2b);

    }
}



