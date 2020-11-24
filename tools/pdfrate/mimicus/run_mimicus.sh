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
echo "Start time: $(date +"%T")"
python reproduction/FT.py > "results_PDFrate_SVM.txt"
echo "Finished running FT.py at $(date +"%T")"
python reproduction/FTC.py > "results_PDFrate_RandomForest.txt"
echo "Finished running FTC.py at $(date +"%T")"