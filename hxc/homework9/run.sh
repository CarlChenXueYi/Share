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
        clang++ agglomerative.cpp Task.cpp showRst.cpp -o  main && ./main -m 3 -t 4
    ;;
    *)
        echo "Usage:$0 somethine.Please see description!"
    ;;
esac