#!/bin/bash


#parallel -u ::: './BJKiller' './BJKiller'  './BJKiller'  './BJKiller'  './BJKiller'  './BJKiller'  './BJKiller'  './BJKiller'  './BJKiller' 

for j in {0..96..8}
do
    for k in {0..72..6}
    do
        for m in {0..72..6}
        do
            for p in {0..48..4}
            do
                echo " A12 T$j M$k L$m S$p"
                ./BJKiller 12 $j $k $m $p
            done
        done
    done
done
