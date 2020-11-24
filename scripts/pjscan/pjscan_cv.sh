#!/bin/bash

clean_dir=$1
dirty_dir=$2
out_dir=$3
n=$4
learning_rate=$5


if [ -z $n ]
then
    echo "ERROR: Invalid input for n"
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

if [ -z $out_dir ]
then
    echo "ERROR: Invalid input for output directory"
    exit
fi

if [ -z $learning_rate ]
then
    learning_rate="0.05"
fi

$(dirname "$0")/cross_validation.sh $1 $2 $3 $4

touch temp.txt

results_dir=$out_dir/results
tests_dir=$results_dir/tests
mkdir $results_dir
mkdir $tests_dir
results=$results_dir/results.txt

for i in `seq 1 $n`
do
    fold_dir=$out_dir/fold_$i
    pjscan learn --nu $learning_rate $fold_dir/fold_${i}_LR$learning_rate.model $fold_dir/train/dirty > temp.txt
    pjscan classify $fold_dir/fold_${i}_LR$learning_rate.model $out_dir/fold_$i/test/composite | grep -E "DIRTY|CLEAN" > $tests_dir/fold_${i}_test_output.txt
    sed -e 's/.*\/composite\///g' -e 's/_\S*//g' -e 's/\[.*\]//g' -e 's/./\U&/g' $tests_dir/fold_${i}_test_output.txt > temp.txt
    tp=`grep "DIRTY MALICIOUS" temp.txt | wc -l`
    tn=`grep "CLEAN BENIGN" temp.txt | wc -l`
    fp=`grep "CLEAN MALICIOUS" temp.txt | wc -l`
    fn=`grep "DIRTY BENIGN" temp.txt | wc -l`
    bu=`expr $(ls -l $fold_dir/test/clean | wc -l) - $(grep "CLEAN" temp.txt | wc -l)`
    mu=`expr $(ls -l $fold_dir/test/dirty | wc -l) - $(grep "DIRTY" temp.txt | wc -l)`


    echo "================================" >> $results
    echo "fold $i" >> $results
    echo >> $results
    echo "TP $tp" >> $results
    echo "TN $tn" >> $results
    echo "FP $fp" >> $results
    echo "FN $fn" >> $results
    echo "BU $bu" >> $results
    echo "MU $mu" >> $results
    echo "================================" >> $results
    echo >> $results
    echo >> $results

    echo "fold $i done"
    echo
    echo
done

tp=`grep "TP" $results | awk '{ sum += $2 } END { print sum }'`
tn=`grep "TN" $results | awk '{ sum += $2 } END { print sum }'`
fp=`grep "FP" $results | awk '{ sum += $2 } END { print sum }'`
fn=`grep "FN" $results | awk '{ sum += $2 } END { print sum }'`
bu=`grep "BU" $results | awk '{ sum += $2 } END { print sum }'`
mu=`grep "MU" $results | awk '{ sum += $2 } END { print sum }'`


echo "================================" >> $results
echo "TOTAL" >> $results
echo >> $results
echo "TP $tp" >> $results
echo "TN $tn" >> $results
echo "FP $fp" >> $results
echo "FN $fn" >> $results
echo "BU $bu" >> $results
echo "MU $mu" >> $results
echo "================================" >> $results

rm temp.txt

echo
echo
echo "cross validation done"
echo
echo