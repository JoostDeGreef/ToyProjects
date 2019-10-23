#!/bin/bash

RELEASE='https://www.sqlite.org/src/tarball/sqlite.tar.gz?r=release'

# todo: convert this to fossil (sqlite git like tool)
if [ ! -d sqlite ];
then
  echo "Downloading SQLite"

  rm -rf download
  mkdir download

  curl ${RELEASE} > download/sqlite.tar.gz

  echo "Unpacking"  
  tar -xzf download/sqlite.tar.gz
  
  echo "Creating amalgation"
  (cd sqlite;sh configure;make sqlite3.c)
fi

#echo "Recreating Makefile.version"
#LONG_VERSION=`cat sqlite/VERSION|head -n 1`
#SHORT_VERSION=`echo ${LONG_VERSION}|cut -d'.' -f 1`
#cat << EOF > Makefile.version
##LONG_VERSION:=.${LONG_VERSION}
##SHORT_VERSION:=.${SHORT_VERSION}
#LONG_VERSION:=
#SHORT_VERSION:=
#EOF

echo "Installing headers"
mkdir -p include/sqlite
rsync sqlite/sqlite3.h include/sqlite/sqlite3.h 


