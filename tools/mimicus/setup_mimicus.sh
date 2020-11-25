#!/bin/bash

# Note: this script assumes that Python 2.7 and Pip are already installed

python $(dirname "$0")/setup.py develop --user
pip install --upgrade setuptools
apt-get install curl perl
pip install matplotlib
pip install numpy
pip install scikit_learn
cd ..