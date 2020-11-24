#!/bin/bash

test_dir=$1

if [ ! -d $test_dir ]
then
    echo "ERROR: Invalid input for test directory"
    exit
fi

echo "file:prediction"
cat $(dirname "$0")/stored_pdfrate_output.txt

