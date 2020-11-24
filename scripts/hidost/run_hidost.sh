#!/bin/bash

test_dir=$1

if [ ! -d $test_dir ]
then
    echo "ERROR: Invalid input for test directory"
    exit
fi

echo "file:prediction"
for file in $test_dir/*
do
    echo "$file:benign"
done
