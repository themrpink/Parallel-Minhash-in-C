#!/bin/bash

main_path=
docs_path=("../docs/docs_small" "../docs/docs_medium" "../docs/docs_big" "../docs/doppi")

usage() { 
            echo "Uso:" 
            echo "-s per versione seriale"
            echo "-o per versione omp"
            echo "-p per versione pthread"
            echo "-c per versione pthread con produttore-consumatore"
            echo '-[ -d <path> ]  directory documenti, default: ["../docs/docs_small" "../docs/docs_medium" "../docs/docs_big" "../docs/doppi"]'
            echo "-h help"
            exit 1 
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

esegui_in_parallelo_OMP() {
    
    IFS=" "
    time_file=$1
    main_path=$2
    for dir in "${docs_path[@]}"; do
        for threads in 1 2 4 8 12 16 32 64; do
            for global_threads in 1 2 4 8; do
                echo "--> esegue $main_path con $threads thread, $global_threads nei nested threads del main cartella documenti: $dir" 
                echo "cartella documenti: $dir" >> ${time_file}
                echo "..."
                echo $( ${main_path} "$dir" "$threads" "$global_threads" ) 
                echo ""
            done;
        done;
    done;
    
}

esegui_in_parallelo_PROD_CONS() {
    
    IFS=" "
    time_file=$1
    main_path=$2
    for dir in "${docs_path[@]}"; do
        echo "--> esegue $main_path con $threads thread, $global_threads nei nested threads del main cartella documenti: $dir" 
        echo "cartella documenti: $dir" >> ${time_file}
        echo "..."
        echo $( ${main_path} "$dir" ) 
        echo ""
    done;
}

esegui_in_serie() {

    IFS=" "
    time_file=$1
    main_path=$2
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

    o)  
        arr+=( "o" )
        ;;

    p)  
        arr+=( "p" ) 
        ;;

    c)      
        arr+=( "c" ) 
        ;;

    d)  
        echo "ok"
        docs_path=( ${OPTARG} )
        if [ -z "$OPTARG" ]; then
            echo "-d: manca il path"
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
        main_path="./pthread_prod_cons"
        time_file="pthread_prod_cons_time_log.txt"   
        echo "pthread producer-consumer"
        esegui_in_parallelo_PROD_CONS $time_file $main_path

    elif [ "$arg" = "s" ]; then
        main_path="./serial"
        time_file="serial_time_log.txt"  
        echo "serial"
        esegui_in_serie $time_file $main_path
    
    elif [ "$arg" = "p" ]; then
        main_path="./pthread"
        time_file="pthread_time_log.txt"
        echo "pthread"
        esegui_in_parallelo $time_file $main_path

    elif [ $arg = "o" ]; then
        main_path="./omp"
        time_file="omp_time_log.txt"
        echo "omp"
        esegui_in_parallelo_OMP $time_file $main_path
    fi

done


exit 0
