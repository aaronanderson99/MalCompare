#!/bin/bash

clean_dir=$1
dirty_dir=$2
out_dir=$3
train_percent=$4
learning_rate=$5

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

if [ -z $out_dir ]
then
    echo "ERROR: Invalid input for output directory"
    exit
fi

if [ -z $train_percent ]
then
    echo "ERROR: Invalid input for train percent"
    exit
fi

if [ -z $learning_rate ]
then
    learning_rate="0.05"
fi

$(dirname "$0")/split.sh $1 $2 $3 $4

touch temp.txt

results_dir=$out_dir/results
mkdir $results_dir
results=$results_dir/results.txt
model="$out_dir/LR$learning_rate.model"

pjscan learn --nu $learning_rate $model $out_dir/train/dirty > temp.txt
pjscan classify $model $out_dir/test/composite | grep -E "DIRTY|CLEAN" > $results_dir/test_output.txt

sed -e 's/.*\/composite\///g' -e 's/_\S*//g' -e 's/\[.*\]//g' -e 's/./\U&/g' $results_dir/test_output.txt > temp.txt
tp=`grep "DIRTY MALICIOUS" temp.txt | wc -l`
tn=`grep "CLEAN BENIGN" temp.txt | wc -l`
fp=`grep "CLEAN MALICIOUS" temp.txt | wc -l`
fn=`grep "DIRTY BENIGN" temp.txt | wc -l`
bu=`expr $(ls -l $out_dir/test/clean | wc -l) - $(grep "CLEAN" temp.txt | wc -l)`
mu=`expr $(ls -l $out_dir/test/dirty | wc -l) - $(grep "DIRTY" temp.txt | wc -l)`

echo "Learning Rate: $learning_rate" >> $results
echo >> $results
echo "TP $tp" >> $results
echo "TN $tn" >> $results
echo "FP $fp" >> $results
echo "FN $fn" >> $results
echo "BU $bu" >> $results
echo "MU $mu" >> $results


rm temp.txt

echo
