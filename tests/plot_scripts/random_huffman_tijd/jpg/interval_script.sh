#!/bin/bash

for blocksize in $(seq 1 304); do
    echo "blocksize: $blocksize" >> jpg_time
    (time cat Earthrise.jpg |AD3zip encodeer 1 $blocksize) >> jpg_$blocksize 2> jpg_time
done