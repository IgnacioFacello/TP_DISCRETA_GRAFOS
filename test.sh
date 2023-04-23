(   date ; 
    echo " $1 $2" ;
    time ./greedy < Grafos/$1.txt
    ) > tests/test_$1\_$2.txt 2>&1
