#!/bin/bash

clean_dir=$1
dirty_dir=$2
out_dir=$3
train_percent=$4

if [ ! -d $clean_dir ]
then
    echo "ERROR: Invalid input for clean samples directory"
    exit
fi

if [ ! -d $dirty_dir ]
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
    train_percent="0.6"
fi

train_dir="${out_dir}/train"
test_dir="${out_dir}/test"

clean_dir_temp="${out_dir}/$(basename ${clean_dir})_temp/"
dirty_dir_temp="${out_dir}/$(basename ${dirty_dir})_temp/"

mkdir "${out_dir}"
mkdir "${train_dir}"
mkdir "${test_dir}"
mkdir "${clean_dir_temp}"
mkdir "${dirty_dir_temp}"

cp -a "$clean_dir/." "${clean_dir_temp}"
cp -a "$dirty_dir/." "${dirty_dir_temp}"

num_clean=`find $clean_dir -maxdepth 1 -type f | wc -l`
num_dirty=`find $dirty_dir -maxdepth 1 -type f | wc -l`

num_clean_train=`echo "($num_clean * $train_percent) / 1" | bc`
num_dirty_train=`echo "($num_dirty * $train_percent) / 1" | bc`

mkdir "${train_dir}/clean"
mkdir "${train_dir}/dirty" 
mkdir "${train_dir}/composite"

mkdir "${test_dir}/clean"
mkdir "${test_dir}/dirty" 
mkdir "${test_dir}/composite"

find "${clean_dir_temp}" -maxdepth 1 -type f | sort | head -n $num_clean_train | xargs -I '{}' mv {} "${train_dir}/clean"
find "${dirty_dir_temp}" -maxdepth 1 -type f | sort | head -n $num_dirty_train | xargs -I '{}' mv {} "${train_dir}/dirty"
cp -a "${train_dir}/clean/." "${train_dir}/composite/"
cp -a "${train_dir}/dirty/." "${train_dir}/composite/"

cp -a "${clean_dir_temp}." "${test_dir}/clean"
cp -a "${dirty_dir_temp}." "${test_dir}/dirty"
cp -a "${test_dir}/clean/." "${test_dir}/composite/"
cp -a "${test_dir}/dirty/." "${test_dir}/composite/"

rm -r ${clean_dir_temp}
rm -r ${dirty_dir_temp}