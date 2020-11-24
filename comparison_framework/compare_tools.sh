#!/bin/bash

test_dir=$1
ground_truth=$2
clean_dir=$3
dirty_dir=$4

has_ground_truth="0"

if [ ! -d $test_dir ]
then
    echo "ERROR: Invalid input for test directory"
    exit
fi

if [ "$ground_truth" = "-gt" ] || [ "$ground_truth" = "--ground-truth" ]
then
    has_ground_truth="1"
fi

if [ "$has_ground_truth" = "1" ] && ( [ -z $clean_dir ] || [ ! -d $clean_dir ] )
then
    echo "ERROR: Invalid input for ground truth benign directory. If no ground truth is available, omit the -gt option."
    exit
fi

if [ "$has_ground_truth" = "1" ] && ( [ -z $dirty_dir ] || [ ! -d $dirty_dir ] )
then
    echo "ERROR: Invalid input for ground truth malicious directory. If no ground truth is available, omit the -gt option."
    exit
fi

echo
echo

$(dirname "$0")/../tools/pjscan/run.sh $test_dir > $(dirname "$0")/pjscan_output.txt
$(dirname "$0")/../tools/pdfrate/run.sh $test_dir > $(dirname "$0")/pdfrate_output.txt
$(dirname "$0")/../tools/slayer/run.sh $test_dir > $(dirname "$0")/slayer_output.txt
$(dirname "$0")/../tools/hidost/run.sh $test_dir > $(dirname "$0")/hidost_output.txt
touch ground_truth.txt

if [ "$has_ground_truth" = "1" ]
then
    $(dirname "$0")/generate_ground_truth.sh $test_dir $clean_dir $dirty_dir > $(dirname "$0")/ground_truth.txt
    python $(dirname "$0")/prediction.py $(dirname "$0")/tool_config.json $(dirname "$0")/ground_truth.txt
else
    python $(dirname "$0")/prediction.py $(dirname "$0")/tool_config.json
fi

echo
echo

rm $(dirname "$0")/pjscan_output.txt
rm $(dirname "$0")/pdfrate_output.txt
rm $(dirname "$0")/slayer_output.txt
rm $(dirname "$0")/hidost_output.txt
rm $(dirname "$0")/ground_truth.txt
