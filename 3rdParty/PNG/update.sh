#!/bin/bash

#set -x

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
mkdir -p ../include/PNG
rsync -a ${SRC}/png.h ../include/PNG/png.h
rsync -a ${SRC}/scripts/pnglibconf.h.prebuilt ../include/PNG/pnglibconf.h

