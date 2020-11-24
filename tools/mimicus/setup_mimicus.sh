#!/bin/bash

# Note: this script assumes that Python 2.7 and Pip are already installed, and that you are currently in the mimicus directory (where this script is located)

python setup.py develop --user
pip install --upgrade setuptools
apt-get install curl perl
pip install matplotlib
pip install numpy
pip install scikit_learn
cd ..