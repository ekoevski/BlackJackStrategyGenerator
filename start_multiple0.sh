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
                echo " A24T${j}M${k}L${m}S${p}"
		if [ ! -f /strategy\ cards/A24H${j}M${k}L${m}S${p}]
		then
                	./BJKiller 24 $j $k $m $p
		fi
            done
        done
    done
done
