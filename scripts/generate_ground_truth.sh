#!/bin/bash

test_dir=$1
clean_dir=$2
dirty_dir=$3

if [ -z $test_dir ] || [ ! -d $test_dir ]
then
    echo "ERROR: Invalid input for test directory"
    exit
fi

if [ -z $clean_dir ] || [ ! -d $clean_dir ]
then
    echo "ERROR: Invalid input for clean samples directory"
    exit
fi

if [ -z $dirty_dir ] || [ ! -d $dirty_dir ]
then
    echo "ERROR: Invalid input for infected samples directory"
    exit
fi

echo "file:prediction"
for file in $test_dir/*
do
    dirty=`ls -l $dirty_dir | grep "$(basename $file)" | wc -l`
    clean=`ls -l $clean_dir | grep "$(basename $file)" | wc -l`
    if [ $dirty -ne 0 ]
    then
        echo "$file:malicious"
    else
        echo "$file:benign"
    fi
done
