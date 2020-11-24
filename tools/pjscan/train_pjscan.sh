#!/bin/bash

train_dir=$1
model=$2
learning_rate=$3

if [ -z $train_dir ] || [ ! -d $train_dir ]
then
    echo "ERROR: Invalid input for training directory"
    exit
fi

if [ -z $model ]
then
    echo "ERROR: Invalid input for model"
    exit
fi

if [ -z $learning_rate ]
then
    learning_rate="0.04"
fi

pjscan learn --nu $learning_rate $model $train_dir
