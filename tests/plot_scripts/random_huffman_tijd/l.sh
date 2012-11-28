#!/bin/bash


cat $1|tr '\n' ' ' |sed 's/s/s\n/g' >> $1_b
rm $1