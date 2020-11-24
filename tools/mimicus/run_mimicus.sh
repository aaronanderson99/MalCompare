#!/bin/bash

# Note: this script assumes that you are currently in the mimicus directory (where this script is located)

test_dir=$1

if [ ! -d $test_dir ]
then
    echo "ERROR: Invalid input for test directory"
    exit
fi

rm results/FC_mimicry/* &> /dev/null
rm results/F_gdkde/* &> /dev/null
rm results/F_mimicry/* &> /dev/null
rm results/FTC_mimicry/* &> /dev/null
rm results/FT_gdkde/* &> /dev/null
rm results/FT_mimicry/* &> /dev/null
python mimicus_generate_attack_list.py $test_dir
python reproduction/FT.py | tee "results_PDFrate_SVM.txt"
#python reproduction/FTC.py | tee "results_PDFrate_RandomForest.txt"
