#!/bin/bash

REPO=https://github.com/glfw/glfw.git
SRC=glfw

if [ ! -d ${SRC} ];
then
  echo "Getting ${Project}"
  git clone ${REPO}
else
  echo "Updating ${Project}"
  git pull
fi

echo "Installing headers"
mkdir -p ../include
rsync -a ${SRC}/include/GLFW/ ../include/GLFW/
