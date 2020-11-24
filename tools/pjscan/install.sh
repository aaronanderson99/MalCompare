#!/bin/bash
### INSTALL DEPENDENCIES
sudo apt-get install cmake libpoppler-dev g++ swig python-dev python-pip python libboost-thread-dev libboost-filesystem-dev libargtable2-dev zip



### INSTALL libpdfjs
cd libpdfjs
mkdir build && cd build
cmake -DMAKE_API=1 -DCMAKE_BUILD_TYPE=Release ..				# NOT -DMAKE_TESTS=1
make
sudo make install
cd ../../
	


### INSTALL libstem
cd libstem
cd build
cmake -DMAKE_API=1 -DCMAKE_BUILD_TYPE=Release ..				# NOT -DMAKE_TESTS=1
make
sudo make install
cd ../../
	


### INSTALL libsvm_oc
cd libsvm_oc
cd build
cmake -DMAKE_API=1 -DCMAKE_BUILD_TYPE=Release ..				# NOT -DMAKE_TESTS=1
make
sudo make install
cd ../../

	

### INSTALL pjscan-js
cd pjscan-js
cd js-1.8.5/js/src
cd build-release
../configure
make
sudo cp ./js /usr/local/bin/pjscan-js



### INSTALL PJScan
cd ../../../../../pjscan
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
