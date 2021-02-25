#!/bin/bash

main_path=
docs_path=("../docs/docs_small" "../docs/docs_big" "../docs/docs_verybig")

usage() { 
            echo "Uso:"
            echo '[ -d <path> ]  directory documenti, default: ["../docs/docs_small" "../docs/docs_medium" "../docs/docs_big"]'
            echo "-s per versione seriale"
            echo "-o per versione omp"
            echo "-p per versione pthread"
            echo "-c per versione pthread con produttore-consumatore"
            echo "-h help"
            exit 0 
}

esegui_in_parallelo() {
    
    IFS=" "
    time_file=$1
    main_path=$2
    for dir in "${docs_path[@]}"; do
        for threads in 1 2 4 8 12 16 32 64; do
            echo "--> esegue $main_path con $threads thread,  cartella documenti: $dir" 
            echo "cartella documenti: $dir" >> ${time_file}
            echo "..."
            echo $( ${main_path} "$dir" "$threads" ) 
            echo ""
        done;
    done;

}


esegui_in_serie() {

    IFS=" "
    docs_path = $1
    main_path = $2
    for dir in "${docs_path[@]}"; do
        echo "--> esegue $main_path,  cartella documenti: $dir" 
        echo "cartella documenti: $dir" >> ${time_file}
        echo "..."
        echo $( ${main_path} "$dir" )
        echo ""
    done;
}


if [ "$#" -eq 0 ]; then
    usage
fi

arr=()

while getopts :sopchd: opt

do
    case "$opt" in
    s)  
        arr+=( "s" )
        ;;

    o)  arr+=( "o" )
        ;;

    p)  
        arr+=( "p" ) 
        ;;

    c)      
        arr+=( "c" ) 
        ;;

    d)  
        docs_path=( ${OPTARG} )
        if [ "$#" -le 2  ]; then
        usage
        fi
        ;;

    h) 
        usage
        ;;

    *) 
        usage
        ;;
    esac
done


for arg in "${arr[@]}"; do

    if [ "$arg" = "c" ]; then
        main_path="../pthread_prod_cons/main"
        time_file="pthread_prod_cons_time_log.txt"   
        echo "pthread producer-consumer"
        esegui_in_parallelo $time_file $main_path

    elif [ "$arg" = "s" ]; then
        main_path="../serial/main"
        time_file="serial_time_log.txt"  
        echo "serial"
        esegui_in_serie $time_file $main_path
    
    elif [ "$arg" = "p" ]; then
        main_path="../pthreads/main"
        time_file="pthread_time_log.txt"
        echo "pthread"
        esegui_in_parallelo $time_file $main_path

    elif [ $arg = "o" ]; then
        main_path="../omp/main"
        time_file="omp_time_log.txt"
        echo "omp"
        esegui_in_parallelo $time_file $main_path
    fi

done


exit 0
