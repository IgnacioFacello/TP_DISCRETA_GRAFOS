(
    echo "----------------------------";
    date ; 
    echo "> $2"; 
    echo -e "\t$1" ;
    time ./greedy < "$1"
 ) >> tests/test.txt 2>&1 ;
echo "[Notice] Test completado"