#!/bin/bash

PROJECT=SPDLog
REPO=https://github.com/gabime/spdlog.git
SRC=spdlog

if [ ! -d ${SRC} ];
then
  echo "Getting ${Project}"
  git clone ${REPO}
else
  echo "Updating ${Project}"
  (cd ${SRC};git checkout .;git pull)
fi

#echo "Installing headers"
#mkdir -p include/gtest
#rsync -a src/gtest/gtest.h include/gtest/gtest.h 
#
#echo "Recreating BuildNumber.txt"
#(cd ${SRC};git tag | grep release | cut -d- -f2 | sort -r --version-sort --field-separator=. | head -n 1) > BuildNumber.txt
