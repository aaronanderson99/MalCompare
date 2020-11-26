#!/bin/bash

### DOWNLOAD CONTAGIO
out_dir=$1

if [ -z $out_dir ]
then
    echo "ERROR: Invalid input for download directory"
    exit
fi

if [ ! -d $out_dir ]
then
    mkdir $out_dir
fi

cd $out_dir


touch temp.txt
wget -O contagio_bulk.zip https://www.dropbox.com/sh/i6ed6v32x0fp94z/AACuZWCgh8EB_r9-fQGvwY2la/BULK?dl=1 > temp.txt
unzip contagio_bulk.zip -d contagio > temp.txt
rm contagio_bulk.zip
cd contagio
unzip -P infected666s CLEAN_PDF_9000_files.zip > temp.txt
unzip -P infected666s MALWARE_PDF_PRE_04-2011_10982_files.zip > temp.txt
mv CLEAN_PDF_9000_files clean
mv MALWARE_PDF_PRE_04-2011_10982_files dirty
rm *.zip *.7z
cd clean
rename "s/\'//g" *
rename "s/ //g" *
cd ../dirty
rm log.txt
rename "s/\'//g" *
rename "s/ //g" *
rename 's/(.*)/$1.pdf/' *
cd ../
mkdir composite
cp -a clean/. composite/
cp -a dirty/. composite/
rm temp.txt
cd ../
rm temp.txt