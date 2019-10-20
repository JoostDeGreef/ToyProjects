#!/bin/bash

REPO=git://git.code.sf.net/p/libpng/code
SRC=code

if [ ! -d ${SRC} ];
then
  echo "Getting ${Project}"
  git clone ${REPO}
else
  echo "Updating ${Project}"
  (cd ${SRC};git pull)
fi

echo "Installing headers"
mkdir -p ../include
rsync -a ${SRC}/include/GLFW/ ../include/GLFW/
