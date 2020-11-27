#!/bin/bash

n=$1

clean_dir=$2
dirty_dir=$3

out_dir=$4

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
    mkdir "${out_dir}/fold$i";
    mkdir "${out_dir}/fold$i/clean"
    mkdir "${out_dir}/fold$i/dirty" 
    mkdir "${out_dir}/fold$i/composite"
    find "${clean_dir_temp}" -maxdepth 1 -type f | head -n $num_clean | xargs -I '{}' mv {} "${out_dir}/fold$i/clean"
    find "${dirty_dir_temp}" -maxdepth 1 -type f | head -n $num_dirty | xargs -I '{}' mv {} "${out_dir}/fold$i/dirty"
    cp -a "${out_dir}/fold$i/clean/." "${out_dir}/fold$i/composite/"
    cp -a "${out_dir}/fold$i/dirty/." "${out_dir}/fold$i/composite/"
done

rm -r ${clean_dir_temp}
rm -r ${dirty_dir_temp}