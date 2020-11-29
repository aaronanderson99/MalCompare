#!/bin/bash

test_dir=$1

if [ ! -d $test_dir ]
then
    echo "ERROR: Invalid input for test directory"
    exit
fi

make -f $(dirname "$0”)/hidost-part-2/Makefile all 

# cat $(dirname "$0")/stored_hidost_output.txt

