#!/bin/bash

#set -x

OUTDIR=processed
DBBASE=${OUTDIR}/ToyProjects
SPLIT=12
INDEX=0
DB=

mkdir -p ${OUTDIR}

rm -f processed/*

# the extensions which are stored
declare -A ext=( ["ttf"]=1 
                 ["otf"]=1
                 ["vs"]=2
                 ["fs"]=3)

IFS='
'

FILEID=0

function UpdateDB() {
  INDEX=$(($INDEX + 1))
  if [ ${INDEX} -ge ${SPLIT} ]
  then
    INDEX=0
  fi
  local ID=`printf %02.0f ${INDEX}`
  DB=`echo "${DBBASE}.${ID}.data"`
}

function ProcessFilelist() {
  local FILELIST=${1}
  local SLOT=${2}
  local BASE=`dirname ${FILELIST}`
  for LINE in `cat ${FILELIST}`
  do
    local NAME=`echo ${LINE}|cut -d= -f 1|xargs`
    local FILE=${BASE}/`echo ${LINE}|cut -d= -f 2|xargs`
    local EXT="${FILE##*.}"
    if [[ -v "ext[${EXT}]" ]]
    then
      local TYPE=${ext[${EXT}]}
      echo "  Adding ${FILE} from ${FILELIST}"
      UpdateDB
      FILEID=$(($FILEID + 1))
      sqlite3 ${DB} "INSERT INTO Data VALUES(${FILEID}, ${SLOT}, ${TYPE}, '${NAME}', ReadFile('${FILE}'));"
    else
      echo "Ignoring ${FILE} from ${FILELIST}"
    fi
  done
}

function ProcessDirectoryContents() {
  local DIR=${1}
  local SLOT=${2}
  if [ -e ${DIR}/filelist ]
  then 
    ProcessFilelist "${DIR}/filelist" ${SLOT}
  else
    UpdateDB
    for D in `find "${DIR}" -mindepth 1 -maxdepth 1 -type d`
    do
      local LEAF=`basename ${D}`
      local ID=`sqlite3 ${DB} "SELECT Id FROM Slot WHERE Slot = ${SLOT} AND Name = '${LEAF}';"`
      ProcessDirectoryContents ${D} ${ID}
    done
    for F in `find "${DIR}" -mindepth 1 -maxdepth 1 -type f`
    do
      local FILE=`basename ${F}`
      local EXT="${FILE##*.}"
      if [[ -v "ext[${EXT}]" ]]
      then
        local TYPE=${ext[${EXT}]}
        local NAME="${FILE%%.*}"
        echo "  Adding ${F}"
        UpdateDB
        FILEID=$(($FILEID + 1))
        sqlite3 ${DB} "INSERT INTO Data VALUES(${FILEID}, ${SLOT}, ${TYPE}, '${NAME}', ReadFile('${F}'));"
      else
        echo "Ignoring ${F}"
      fi
    done
  fi
}

function ProcessDirectory() {
  local DIR=${1}
  local SLOT=${2}
  if [ ! -e ${DIR}/filelist ]
  then 
    for D in `find "${DIR}" -mindepth 1 -maxdepth 1 -type d`
    do
      local LEAF=`basename ${D}`
      local ID=`sqlite3 ${DB} "INSERT INTO Slot VALUES(NULL,${SLOT},'${LEAF}'); SELECT Id FROM Slot WHERE Slot = ${SLOT} AND Name = '${LEAF}';"`
      ProcessDirectory ${D} ${ID}
    done
  fi
}

function Run() {
  local DIR=${1}
  for ID in `seq 1 ${SPLIT}`
  do
    UpdateDB
    echo "Creating ${DB}"
    sqlite3 ${DB} << EOF
CREATE TABLE Type(Id INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE Slot(Id INTEGER PRIMARY KEY AUTOINCREMENT, Slot INTEGER, Name TEXT);
CREATE TABLE Data(Id INTEGER PRIMARY KEY, Slot INTEGER, Type INTEGER, Name TEXT, Data BLOB);

INSERT INTO Type
     VALUES(1,'Font'),
           (2,'Vertex Shader'),
           (3,'Fragment Shader');
INSERT INTO Slot
     VALUES(0, -1, ''); -- root
EOF
    ProcessDirectory ${DIR} 0 
  done
  ProcessDirectoryContents ${DIR} 0
}

Run "input"
