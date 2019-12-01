#!/bin/bash

if [ -f tmp/glad.zip ]
then
  echo "GLAD already downloaded"
else
  echo "Generating GLAD package"

  LINK=`
  curl -X POST \
       --data "language=c&specification=gl&api=gl%3D3.3&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on" \
       https://glad.dav1d.de/generate | \
  grep -o '<a href=['"'"'"][^"'"'"']*['"'"'"]' | \
  sed -e 's/^<a href=["'"'"']//' -e 's/["'"'"']$//' `

  LINK="https://glad.dav1d.de${LINK}/glad.zip" 

  mkdir -p tmp
  curl ${LINK} > tmp/glad.zip
fi

if [ ! -d glad ]
then
  echo "Unpacking package"
  rm -rf glad
  mkdir glad
  (cd glad;unzip ../tmp/glad.zip)
fi 
