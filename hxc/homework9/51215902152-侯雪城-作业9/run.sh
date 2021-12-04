################################################################################
# Description: 
# Parameter  : 
# Author     : houxuecheng
# Date       : 2021/11/18/11:50
# Version    : 1.0
# Modifiy    : 
################################################################################
 
 
 
 
case "$1" in
    run)
        clang++ agglomerative.cpp Task.cpp showRst.cpp -o  main && ./main -n 2 -t 4 -m 1 && rm -rf main;
        clang++ agglomerative.cpp Task.cpp showRst.cpp -o  main && ./main -n 2 -t 4 -m 2 && rm -rf main;
        clang++ agglomerative.cpp Task.cpp showRst.cpp -o  main && ./main -n 2 -t 4 -m 3 && rm -rf main
    ;;
    *)
        echo "Usage:$0 somethine.Please see description!"
    ;;
esac