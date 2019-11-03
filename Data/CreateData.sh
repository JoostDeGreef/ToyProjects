#!/bin/bash

#set -x

OUTDIR=../out/Data
DB=${OUTDIR}/ToyProject.data

mkdir -p ${OUTDIR}

rm -f ${DB}

sqlite3 ${DB} << EOF
CREATE TABLE Type(Id INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT);
CREATE TABLE Slot(Id INTEGER PRIMARY KEY AUTOINCREMENT, Slot INTEGER, Name TEXT);
CREATE TABLE Data(Id INTEGER PRIMARY KEY AUTOINCREMENT, Slot INTEGER, Type INTEGER, Name TEXT, Data BLOB);

INSERT INTO Type
     VALUES(1,'Font'),
	       (2,'Vertex Shader'),
	       (3,'Fragment Shader');
INSERT INTO Slot
     VALUES(0, -1, ''); -- root
EOF

# the extensions which are stored
declare -A ext=( ["ttf"]=1 
                 ["otf"]=1
				 ["vs"]=2
				 ["fs"]=3)

IFS='
'

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
      sqlite3 ${DB} "INSERT INTO Data VALUES(NULL, ${SLOT}, ${TYPE}, '${NAME}', ReadFile('${FILE}'));"
	else
	  echo "Ignoring ${FILE} from ${FILELIST}"
	fi
  done
}

function ProcessDirectory() {
  local DIR=${1}
  local SLOT=${2}
  if [ -e ${DIR}/filelist ]
  then 
    ProcessFilelist "${DIR}/filelist" ${SLOT}
  else
    for D in `find "${DIR}" -mindepth 1 -maxdepth 1 -type d`
	do
	  local LEAF=`basename ${D}`
      local ID=`sqlite3 ${DB} "INSERT INTO Slot VALUES(NULL,${SLOT},'${LEAF}'); SELECT Id FROM Slot WHERE Slot = ${SLOT} AND Name = '${LEAF}';"`
	  ProcessDirectory ${D} ${ID}
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
        sqlite3 ${DB} "INSERT INTO Data VALUES(NULL, ${SLOT}, ${TYPE}, '${NAME}', ReadFile('${FILE}'));"
  	  else
	    echo "Ignoring ${F}"
	  fi
	done
  fi
}

ProcessDirectory "." 0

