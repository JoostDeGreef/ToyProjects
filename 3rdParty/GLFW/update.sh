#!/bin/bash

REPO=https://github.com/glfw/glfw.git
SRC=glfw

if [ ! -d ${SRC} ];
then
  echo "Getting ${Project}"
  git clone ${REPO}
else
  echo "Updating ${Project}"
  (cd ${SRC};git pull)
fi

#sed -i glfw/CMakeLists.txt -e "s|set(GLFW_LIB_NAME.*$|set(GLFW_LIB_NAME GLFW)|g"
#echo "include(Library)" >> glfw/CMakeLists.txt
#echo "add_library (GLFW \"glad/src/glad.c\")" >> glfw/CMakeLists.txt

#echo "Installing headers"
#mkdir -p ../include
#rsync -a ${SRC}/include/GLFW/ ../include/GLFW/
