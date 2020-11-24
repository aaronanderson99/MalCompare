#!/bin/bash

test_dir=$1
model=$2

if [ ! -d $test_dir ]
then
    echo "ERROR: Invalid input for test directory"
    exit
fi

if [ -z model ]
then
    echo "ERROR: Invalid input for test model"
    exit
fi

pjscan classify $model $test_dir | grep ".pdf" > pjscan_test_output_temp.txt

echo "file:prediction"
for f in $test_dir/*
do
    is_malicious=`grep "$(basename "$f")" pjscan_test_output_temp.txt | grep "malicious" | wc -l`
    is_benign=`grep "$(basename "$f")" pjscan_test_output_temp.txt | grep "benign" | wc -l`

    if [ "$is_malicious" != "0" ]
    then
        echo "$(basename "$f"):malicious"
    else
        echo "$(basename "$f"):benign"
    fi
done

rm pjscan_test_output_temp.txt
