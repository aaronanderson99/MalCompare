#!/bin/bash
### INSTALL DEPENDENCIES
sudo apt-get install cmake libpoppler-dev g++ swig python-dev python-pip python libboost-thread-dev libboost-filesystem-dev libargtable2-dev zip


out_dir=$1

if [ -z $out_dir ]
then
    echo "ERROR: Invalid input for pjscan install directory"
    exit
fi

if [ ! -d $out_dir ]
then
    mkdir $out_dir
fi

cd $out_dir



### DOWNLOAD PACKAGE
svn checkout http://svn.code.sf.net/p/pjscan/code/trunk pjscan			# NOT https
cd pjscan



### INSTALL libpdfjs
sudo rm -r libpdfjs	
svn checkout http://svn.code.sf.net/p/libpdfjs/code/trunk libpdfjs		# NOT https
cd libpdfjs
mkdir build && cd build
cmake -DMAKE_API=1 -DCMAKE_BUILD_TYPE=Release ..				# NOT -DMAKE_TESTS=1
make
sudo make install
cd ../../
	


### INSTALL libstem
cd libstem
mkdir build && cd build
cmake -DMAKE_API=1 -DCMAKE_BUILD_TYPE=Release ..				# NOT -DMAKE_TESTS=1
make
sudo make install
cd ../../
	


### INSTALL libsvm_oc
cd libsvm_oc
mkdir build && cd build
cmake -DMAKE_API=1 -DCMAKE_BUILD_TYPE=Release ..				# NOT -DMAKE_TESTS=1
make
sudo make install
cd ../../

	

### INSTALL pjscan-js
cd pjscan-js
wget http://ftp.mozilla.org/pub/mozilla.org/js/js185-1.0.0.tar.gz
tar xvfz js185-1.0.0.tar.gz
cd js-1.8.5/js/src
patch <../../../pjscan-js.patch			# NOT patch <../../../jsscan.h.patch && patch <../../../jsscan.cpp.patch
mkdir build-release && cd build-release
../configure
make
sudo cp ./js /usr/local/bin/pjscan-js



### INSTALL PJScan
cd ../../../../../pjscan
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
