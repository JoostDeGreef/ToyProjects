#!/bin/bash

REPO=https://github.com/google/googletest.git 
SRC=googletest
PYTHON=`which python`
if [ -z ${PYTHON} ];
then
  echo "Python should be accesible!"
  exit 1
fi

if [ ! -d googletest ];
then
  echo "Getting ${Project}"
  git clone ${REPO}
else
  echo "Updating ${Project}"
  git pull
fi

echo "Rebuilding fused src"
rm -rf src
python ./googletest/googletest/scripts/fuse_gtest_files.py src 

echo "Installing headers"
rm -rf ../include/gtest
mkdir -p ../include/gtest
cp src/gtest/gtest.h ../include/gtest

