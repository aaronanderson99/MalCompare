#!/bin/bash

test_dir=$1
option_1=$2
arg_1=$3
option_2=$4
arg_2=$5

ground_truth_option="0"
query_option="0"

ground_truth="no_ground_truth"
query="no_query"

if [ ! -d $test_dir ]
then
    echo "ERROR: Invalid input for test directory"
    exit
fi


if [ "$option_1" = "-gt" ] || [ "$option_1" = "--ground-truth" ]
then
    ground_truth_option="1"
    ground_truth="$arg_1"
fi

if [ "$option_1" = "-q" ] || [ "$option_1" = "--query" ]
then
    query_option="1"
    query="$arg_1"
fi

if [ "$option_2" = "-gt" ] || [ "$option_2" = "--ground-truth" ]
then
    ground_truth_option="1"
    ground_truth="$arg_2"
fi

if [ "$option_2" = "-q" ] || [ "$option_2" = "--query" ]
then
    query_option="1"
    query="$arg_2"
fi


if [ "$ground_truth_option" = "1" ] && [ ! -f "$ground_truth" ]
then
    echo "ERROR: Invalid input for ground truth. If no ground truth is available, omit the -gt option."
    exit
fi

if [ "$query_option" = "1" ] && [ -z "$query" ]
then
    echo "ERROR: Invalid input for query."
    exit
fi

echo
echo

$(dirname "$0")/../tools/pjscan/run.sh $test_dir > pjscan_output.txt
$(dirname "$0")/../tools/pdfrate/run.sh $test_dir > pdfrate_output.txt
$(dirname "$0")/../tools/slayer/run.sh $test_dir > slayer_output.txt
$(dirname "$0")/../tools/hidost/run.sh $test_dir > hidost_output.txt

python $(dirname "$0")/malcompare.py $(dirname "$0")/tool_config.json "$ground_truth" "$query"

echo
echo

rm pjscan_output.txt
rm pdfrate_output.txt
rm slayer_output.txt
rm hidost_output.txt
