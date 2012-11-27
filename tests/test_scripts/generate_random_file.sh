#!/bin/bash
for size in $(seq 20); do
    dd if=/dev/urandom of=file_$size bs=1M count=$size
done
