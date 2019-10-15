#!/bin/bash

REPO=https://github.com/google/googletest.git 
SRC=googletest

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
winpty python ./googletest/googletest/scripts/fuse_gtest_files.py src 

echo "Recreating Makefile.version"
LONG_VERSION=`cat googletest/googletest/CHANGES | grep -o -e '[1-9]\.[0-9]\.[0-9]' | head -n 1`
SHORT_VERSION=`echo ${LONG_VERSION}|cut -d'.' -f 1`
cat << EOF > Makefile.version
#LONG_VERSION:=.${LONG_VERSION}
#SHORT_VERSION:=.${SHORT_VERSION}
LONG_VERSION:=
SHORT_VERSION:=
EOF

echo "Installing headers"
rm -rf ../include/gtest
mkdir -p ../include/gtest
cp googletest/googletest/include/gtest/gtest.h ../include/gtest

