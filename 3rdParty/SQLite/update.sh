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
  
  echo "Recreating SQLiteVersion.cmake"
  VERSION=`cat sqlite/VERSION|head -n 1`
  cat << EOF > SQLiteVersion.cmake
set(SQLITE_VERSION ${VERSION})
EOF
fi

echo "Recreating SQLiteVersion.cmake"
VERSION=`cat sqlite/VERSION|head -n 1`
cat << EOF > SQLiteVersion.cmake
set(SQLITE_VERSION ${VERSION})
EOF

echo "Installing headers"
mkdir -p include/sqlite
rsync sqlite/sqlite3.h include/sqlite/sqlite3.h 


