#!/bin/bash

BIN_PATH=../..
TEST_FILE_PATH=../../tests_no_git

echo "Test voor Burrows-Wheeler"
for blocksize in $(seq 10); do
    dd if=/dev/urandom bs=1 count=10000 of=/dev/stdout 2> /dev/null |valgrind $BIN_PATH/AD3zip encodeer 0 $blocksize 2>&1 |sed -n '/^ERROR\|heap/p'
    dd if=/dev/urandom bs=1 count=10000 of=/dev/stdout 2> /dev/null |$BIN_PATH/AD3zip encodeer 0 $blocksize |valgrind $BIN_PATH/AD3zip decodeer 2>&1 |sed -n '/^ERROR\|heap/p'
done

echo "Test voor Standaard huffman"
for blocksize in $(seq 10); do
    dd if=/dev/urandom bs=1 count=10000 of=/dev/stdout 2> /dev/null |valgrind $BIN_PATH/AD3zip encodeer 1 $blocksize 2>&1 |sed -n '/^ERROR\|heap/p'
    dd if=/dev/urandom bs=1 count=10000 of=/dev/stdout 2> /dev/null |$BIN_PATH/AD3zip encodeer 1 $blocksize |valgrind $BIN_PATH/AD3zip decodeer 2>&1 |sed -n '/^ERROR\|heap/p'
done

echo "Test voor LZ7"
for blocksize in $(seq 10); do
    dd if=/dev/urandom bs=1 count=10000 of=/dev/stdout 2> /dev/null |valgrind $BIN_PATH/AD3zip encodeer 4 $blocksize 2>&1 |sed -n '/^ERROR\|heap/p'
    dd if=/dev/urandom bs=1 count=10000 of=/dev/stdout 2> /dev/null |$BIN_PATH/AD3zip encodeer 4 $blocksize |valgrind $BIN_PATH/AD3zip decodeer 2>&1 |sed -n '/^ERROR\|heap/p'
done