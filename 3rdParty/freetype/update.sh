#!/bin/bash

WEB='https://download.savannah.gnu.org/releases/freetype/'

FILE=`
curl -s ${WEB} | \
grep -o '<a href=['"'"'"][^"'"'"']*['"'"'"]' | \
sed -e 's/^<a href=["'"'"']//' -e 's/["'"'"']$//' | \
egrep -o 'freetype-([0-9])*\.([0-9])*\.([0-9])*.tar.bz2' | \
sort -r | \
head -n 1`

if [ -z "$FILE" ];
then
  echo "No freetype tar.bz2 available for download in ${WEB}??"
  exit 1
fi

if [ ! -f download/${FILE} ];
then
  echo -e "Downloading FreeType: ${FILE}\n(from: ${WEB})"

  rm -rf download
  mkdir download

  (cd download;wget "${WEB}${FILE}") || exit 1
fi

if [ ! -d freetype ];
then
  echo "Unpacking"  
  
  rm -rf tmp
  mkdir tmp
  (cd tmp;tar -xjf ../download/${FILE}) || exit 1

  mv tmp/$(basename ${FILE} .tar.bz2) freetype   
fi

#echo "Installing headers"
#mkdir -p ../include/
#rsync -a freetype/include/freetype/ ../include/freetype/

