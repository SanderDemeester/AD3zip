#!/bin/bash

for blocksize in $(seq 500 500 9000); do
    echo "blocksize: $blocksize" >> random_a/result_time_a
    (time cat 50M |AD3zip encodeer 1 $blocksize) >> random_a/a_$blocksize 2> random_a/result_time_a
done