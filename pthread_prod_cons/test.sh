#!/bin/bash

for x in 1 2 4 8 16 32 64 128; do   
    var=$(./main "$1" "$x")
    echo $x $1 $var
    #echo $var | grep -q "coerenza delle signatures" && echo "errore" || echo "corretto"
done;
