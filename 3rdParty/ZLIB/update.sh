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

echo "Installing headers"
#mkdir -p ../include
#rsync -a ${SRC}/include/GLFW/ ../include/GLFW/
