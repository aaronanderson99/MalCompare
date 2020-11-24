#!/bin/bash

clean_dir=$1
dirty_dir=$2
out_dir=$3
n=$4

if [ -z $n ]
then
    n="4"
fi

if [ -z $clean_dir ] || [ ! -d $clean_dir ]
then
    echo "ERROR: Invalid input for clean samples directory"
    exit
fi

if [ -z $clean_dir ] || [ ! -d $dirty_dir ]
then
    echo "ERROR: Invalid input for infected samples directory"
    exit
fi

if [ -z $out_dir ]
then
    echo "ERROR: Invalid input for output directory"
    exit
fi

clean_dir_temp="${out_dir}/$(basename ${clean_dir})_temp/"
dirty_dir_temp="${out_dir}/$(basename ${dirty_dir})_temp/"

mkdir "${out_dir}"
mkdir "${clean_dir_temp}"
mkdir "${dirty_dir_temp}"

cp -a "$clean_dir/." "${clean_dir_temp}"
cp -a "$dirty_dir/." "${dirty_dir_temp}"

num_clean=$((`find $clean_dir -maxdepth 1 -type f | wc -l`/$n+1))
num_dirty=$((`find $dirty_dir -maxdepth 1 -type f | wc -l`/$n+1))


for i in `seq 1 $n`;
do
    fold_dir="${out_dir}/fold_$i"
    train_dir="${fold_dir}/train"
    test_dir="${fold_dir}/test"

    mkdir $fold_dir
    mkdir $train_dir
    mkdir $test_dir

    mkdir "${train_dir}/clean";
    mkdir "${train_dir}/dirty";
    mkdir "${train_dir}/composite";
    
    mkdir "${test_dir}/clean";
    mkdir "${test_dir}/dirty";
    mkdir "${test_dir}/composite";

    find "${clean_dir_temp}" -maxdepth 1 -type f | head -n $num_clean | xargs -I '{}' mv {} "${test_dir}/clean"
    find "${dirty_dir_temp}" -maxdepth 1 -type f | head -n $num_dirty | xargs -I '{}' mv {} "${test_dir}/dirty"
    cp -a "${test_dir}/clean/." "${test_dir}/composite/"
    cp -a "${test_dir}/dirty/." "${test_dir}/composite/"
done

for i in `seq 1 $n`;
do
    for j in `seq 1 $n`
    do
        if [ $i -ne $j ]
        then
            cp -a "${out_dir}/fold_$j/test/clean/." "${out_dir}/fold_$i/train/clean/"
            cp -a "${out_dir}/fold_$j/test/dirty/." "${out_dir}/fold_$i/train/dirty/"
        fi
    done
    cp -a "${out_dir}/fold_$i/train/clean/." "${out_dir}/fold_$i/train/composite/"
    cp -a "${out_dir}/fold_$i/train/dirty/." "${out_dir}/fold_$i/train/composite/"
done

rm -r ${clean_dir_temp}
rm -r ${dirty_dir_temp}