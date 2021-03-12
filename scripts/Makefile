foo := who are you
fo2 := file non compilato
bar := $(if $? -ne 0,echo $(foo),$(fo2)!)
all: serial omp pthread_prod_cons pthread


serial: 
	gcc -Wall -fopenmp -o serial ../serial/main.c ../serial/documents_getters.c               \
                                    ../serial/get_similarities.c ../serial/hash_FNV_1.c       \
                                    ../serial/tokenizer.c ../serial/shingle_extract.c         \
                                    ../serial/time_test.c
	@echo creato file eseguibile serial

omp: 
	gcc -Wall -fopenmp -o omp ../omp/main.c ../omp/documents_getters.c                  \
                                    ../omp/get_similarities.c ../omp/hash_FNV_1.c       \
                                    ../omp/tokenizer.c ../omp/shingle_extract.c         \
                                    ../omp/time_test.c
	@echo creato file eseguibile omp

pthread_prod_cons: 
	gcc -Wall -fopenmp -o pthread_prod_cons ../pthread_prod_cons/main.c ../pthread_prod_cons/documents_getters.c    \
                                    ../pthread_prod_cons/get_similarities.c ../pthread_prod_cons/hash_FNV_1.c       \
                                    ../pthread_prod_cons/tokenizer.c ../pthread_prod_cons/shingle_extract.c         \
                                    ../pthread_prod_cons/time_test.c ../pthread_prod_cons/queue.c                   \
                                    ../pthread_prod_cons/prod_cons.c ../pthread_prod_cons/minhash.c
	@echo creato file eseguibile prod_cons

pthread: 
	gcc -Wall -fopenmp -o  pthread ../pthreads/main.c ../pthreads/documents_getters.c              \
                                    ../pthreads/get_similarities.c ../pthreads/hash_FNV_1.c       \
                                    ../pthreads/tokenizer.c ../pthreads/shingle_extract.c         \
                                    ../pthreads/time_test.c
	@echo creato file eseguibile pthread


clean:
	rm -f pthread omp pthread_prod_cons serial


clean_test:
	rm *.txt *.csv
