#!/bin/bash

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
  (cd ${SRC};git checkout .;git pull)
fi

echo "Rebuilding fused src"
rm -rf src
python ./googletest/googletest/scripts/fuse_gtest_files.py src 

echo "Installing headers"
mkdir -p include/gtest
rsync -a src/gtest/gtest.h include/gtest/gtest.h 

echo "Recreating BuildNumber.txt"
(cd ${SRC};git tag | grep release | cut -d- -f2 | sort -r --version-sort --field-separator=. | head -n 1) > BuildNumber.txt
