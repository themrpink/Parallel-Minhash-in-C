
#ifndef TIME_TEST_  /* Include guard */
#define TIME_TEST_

enum Tasks {   Serial_signatures, 
               Serial_shingles, 
               Parallel_signatures, 
               Parallel_shingles, 
               Any
          };

void exectimes(double value, enum Tasks task,int print);


#endif