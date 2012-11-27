#!/bin/bash

for file_size in $(seq 20); do
    for block_size in $(seq 5); do
	echo "filesize: $file_size && blocksize: $block_size" >> result.r
	(time AD3zip encodeer 1 $block_size < file_$file_size) >> /dev/null 2>> result.r
    done
done