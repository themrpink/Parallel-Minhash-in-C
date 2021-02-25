#!/bin/bash

main_path=
numb_threads=1
while getopts :sopch: opt

do
    case "$opt" in
    s)  
        main_path="../serial/main"
        numb_threads=0
        time_file="serial_time_log.txt"
        echo "serial";;

    o)  
        main_path="../omp/main"
        time_file="omp_time_log.txt"
        echo "omp";;

    p)  
        main_path="../pthreads/main"
        time_file="pthread_time_log.txt"
        echo "pthread";;

    c)  
        main_path="../pthread_prod_cons/main"
        time_file="pthread_prod_cons_time_log.txt"
        echo "pthread producer consumer";;

    *) 
        echo "opzione $opt sconosciuta. Opzioni:"
        echo "-s per versione seriale"
        echo "-o per versione omp"
        echo "-p per versione pthread"
        echo "-c per versione pthread con produttore-cosumatore"
        exit 0
        ;;
    esac
done
IFS=" "

if [ $numb_threads -eq 1 ]
then
    for docs_path in "../docs/docs_small" "../docs/docs_big" "../docs/docs_verybig"; do
        for threads in 1 2 4 8 12 16 32 64; do
            echo "--> esegue $main_path con $threads thread,  cartella documenti: $docs_path" 
            echo "cartella documenti: $docs_path" >> ${time_file}
            echo "..."
            echo $( ${main_path} "$docs_path" "$threads" ) 
            echo ""
        done;
    done;

else
    for docs_path in "../docs/docs_small" "../docs/docs_big" "../docs/docs_verybig"; do
        echo "--> esegue $main_path,  cartella documenti: $docs_path" 
        echo "cartella documenti: $docs_path" >> ${time_file}
        echo "..."
        echo $( ${main_path} "$docs_path" )
        echo ""
    done;
fi

exit 0
