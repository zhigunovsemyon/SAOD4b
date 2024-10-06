#!/bin/bash

for i in `seq 1 10` 
do
    time ./out/build/linux-debug/SAOD4 $1 | grep Шагов
    echo ''
done

