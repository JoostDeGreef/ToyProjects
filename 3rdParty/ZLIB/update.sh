#!/bin/bash

REPO=https://github.com/madler/zlib.git
SRC=zlib

if [ ! -d ${SRC} ];
then
  echo "Getting ${Project}"
  git clone ${REPO}
else
  echo "Updating ${Project}"
  (cd ${SRC};git pull)
fi

#sed -i zlib/CMakeLists.txt -e 's|set(CMAKE_DEBUG_POSTFIX "d")||g'

#echo "Installing headers"
#mkdir -p include/ZLIB
#rsync -a ${SRC}/zlib.h include/ZLIB/zlib.h
#rsync -a ${SRC}/zconf.h include/ZLIB/zconf.h

