#!/bin/bash

if [ -f tmp/glad.zip ] && [ -f include/GLAD/glad.h ]
then
  echo "GLAD already downloaded"
else
  echo "Generating GLAD package"

  LINK=`
  curl -X POST \
       --data "language=c&specification=gl&api=gl%3D4.0&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on" \
       https://glad.dav1d.de/generate | \
  grep -o '<a href=['"'"'"][^"'"'"']*['"'"'"]' | \
  sed -e 's/^<a href=["'"'"']//' -e 's/["'"'"']$//' `

  LINK="https://glad.dav1d.de${LINK}/glad.zip" 

  mkdir -p tmp
  curl ${LINK} > tmp/glad.zip

  echo "Unpacking package"
  rm -rf glad
  mkdir glad
  (cd glad;unzip ../tmp/glad.zip)
 
#  echo "Installing headers"
#  mkdir -p include/GLAD include/KHR
#  rsync -a glad/include/glad/ include/GLAD/
#  rsync -a glad/include/KHR/ include/KHR/
fi

