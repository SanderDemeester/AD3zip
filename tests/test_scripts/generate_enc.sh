#!/bin/bash

find . -name "file*" | while read file; do
    for size in $(seq 5); do
	cat $file |AD3zip encodeer 1 $size >> $file$size.enc
    done
done
