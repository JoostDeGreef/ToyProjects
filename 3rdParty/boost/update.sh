#!/bin/bash

set -e

VERSIONS=`
curl -s https://dl.bintray.com/boostorg/release/ | \
grep -o '<a href=['"'"'"][^"'"'"']*['"'"'"]' | \
sed -e 's/^<a href=["'"'"']//' -e 's/["'"'"']$//' | \
egrep -o '([0-9])*\.([0-9])*\.([0-9])*' | \
sort -r | \
head -n 10`

echo -e "Available versions:\n${VERSIONS}"

FILE=""
for VERSION in ${VERSIONS}
do
  FILE=`
    curl -s https://dl.bintray.com/boostorg/release/${VERSION}/source/ | \
    grep -o '<a href=['"'"'"][^"'"'"']*['"'"'"]' | \
    sed -e 's/^<a href=["'"'"']//' -e 's/["'"'"']$//' | \
    grep '.tar.gz$' | \
    grep -v "_rc" | \
    sort -r | \
    head -n 1`
  if [ -n "${FILE}" ];
  then
    LINK=https://dl.bintray.com/boostorg/release/${VERSION}/source/${FILE} 
    break
  fi  
done

if [ -z "${FILE}" ];
then
  echo "No stable version available for download?"
  exit 1
fi

echo "Latest archive: ${FILE}"

if [ -f tmp/${FILE} ]
then
  echo "Already downloaded tmp/${FILE}"
else
  mkdir -p tmp
  (cd tmp;wget ${LINK})
fi

BASE=`basename ${FILE} .tar.gz`
if [ ! -d tmp/${BASE} ]
then
  (cd tmp;rm -f ${BASE};tar -xzf ${FILE})
fi
rsync -a tmp/${BASE}/ boost/

#echo "Recreating Makefile.version"
#LONG_VERSION=`echo ${FILE}|cut -d'.' -f 1|rev|cut -d'_' -f1-3|rev|tr '_' '.'`
#SHORT_VERSION=`echo ${LONG_VERSION}|cut -d'.' -f 1`
#cat << EOF > Makefile.version
##LONG_VERSION:=.${LONG_VERSION}
##SHORT_VERSION:=.${SHORT_VERSION}
#LONG_VERSION:=
#SHORT_VERSION:=
#EOF

echo "Installing headers"
mkdir -p include/
rsync -a boost/boost/ include/boost/


