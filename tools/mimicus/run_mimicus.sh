#!/bin/bash

test_dir=$1

if [ ! -d $test_dir ]
then
    echo "ERROR: Invalid input for test directory"
    exit
fi

rm $(dirname "$0")/results/FC_mimicry/* &> /dev/null
rm $(dirname "$0")/results/F_gdkde/* &> /dev/null
rm $(dirname "$0")/results/F_mimicry/* &> /dev/null
rm $(dirname "$0")/results/FTC_mimicry/* &> /dev/null
rm $(dirname "$0")/results/FT_gdkde/* &> /dev/null
rm $(dirname "$0")/results/FT_mimicry/* &> /dev/null
python $(dirname "$0")/mimicus_generate_attack_list.py $test_dir
python $(dirname "$0")/reproduction/FT.py | tee "results_PDFrate_SVM.txt"
#python $(dirname "$0")/reproduction/FTC.py | tee "results_PDFrate_RandomForest.txt"
