#!/bin/bash

#set -x 

REPO=https://github.com/google/googletest.git 
SRC=googletest
PYTHON=`which python`
if [ -z ${PYTHON} ];
then
  echo "Python should be accesible as 'python'."
  exit 1
fi

if [ ! -d ${SRC} ];
then
  echo "Getting ${Project}"
  git clone ${REPO}
else
  echo "Updating ${Project}"
  (cd ${SRC};git pull)
fi

echo "Rebuilding fused src"
rm -rf src
python ./googletest/googletest/scripts/fuse_gtest_files.py src 

echo "Installing headers"
mkdir -p ../include/gtest
diff -q src/gtest/gtest.h ../include/gtest/gtest.h > /dev/null 2>&1
if [ $? != 0 ]
then
  cp src/gtest/gtest.h ../include/gtest/gtest.h
fi
diff -q src/gtest/gtest-all.cc src-extra/gtest-all.cc > /dev/null 2>&1
if [ $? != 0 ]
then
  cp src/gtest/gtest-all.cc src-extra/gtest-all.cc
fi

