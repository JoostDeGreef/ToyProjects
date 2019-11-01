#!/bin/bash

#set -x

OUTDIR=../out/Data
DB=${OUTDIR}/OpenGLTest.data

mkdir -p ${OUTDIR}

rm -f ${DB}

sqlite3 ${DB} << EOF
CREATE TABLE Slot(Id INTEGER PRIMARY KEY AUTOINCREMENT, SlotId INTEGER, Name TEXT);
CREATE TABLE Data(Id INTEGER PRIMARY KEY AUTOINCREMENT, SlotId INTEGER, Name TEXT, Data BLOB);

INSERT INTO Slot VALUES(0, -1, 'ROOT');
EOF

function GetSlotID() {
  local SLOT=${1}
  local BASE=0
  local LEAF=`basename "${SLOT}"`
  if [ "${LEAF}" != "${SLOT}" ];
  then
    DIR=`dirname "${SLOT}"`
    BASE=`GetSlotID "${DIR}"`
  fi
  local ID=`sqlite3 ${DB} "SELECT Id FROM Slot WHERE SlotId = ${BASE} AND Name = '${LEAF}';"`
  if [ -z ${ID} ]
  then
    ID=`sqlite3 ${DB} "INSERT INTO Slot VALUES(NULL,${BASE},'${LEAF}'); SELECT Id FROM Slot WHERE SlotId = ${BASE} AND Name = '${LEAF}';"`
  fi
  echo ${ID}
}

IFS='
'
for FILELIST in `find * -name filelist`
do
  BASE=`dirname ${FILELIST}`
  for LINE in `cat ${FILELIST}`
  do
    NAME=`echo ${LINE}|cut -d= -f 1|xargs`
    FILE=${BASE}/`echo ${LINE}|cut -d= -f 2|xargs`
    SLOT=`dirname "${FILE}"`
    SLOTID=`GetSlotID ${SLOT}`
    sqlite3 ${DB} << EOF
INSERT INTO Data VALUES(NULL, ${SLOTID}, '${NAME}', ReadFile('${FILE}'));
EOF
  done
done




#sqlite3 ${OUTDIR}/OpenGLTest.data << EOF
#CREATE TABLE Fonts(Id INTEGER, Name TEXT, Data BLOB);
#
#INSERT INTO Fonts
#  (Id,Name,Data) 
#VALUES
#  ( 0, 'Comic Jens',       ReadFile('./fonts/comic-jens/ComicJensFreePro-Regular.ttf')), 
#  ( 1, 'Exo Black Italic', ReadFile('./fonts/exo/Exo-Black-Italic.otf')), 
#  ( 2, 'Exo Black',        ReadFile('./fonts/exo/Exo-Black.otf')), 
#  ( 3, 'Exo Italic',       ReadFile('./fonts/exo/Exo-Regular-Italic.otf')), 
#  ( 4, 'Exo',              ReadFile('./fonts/exo/Exo-Regular.otf')), 
#  ( 5, 'Infini Bold',      ReadFile('./fonts/infini/infini-gras.otf')), 
#  ( 6, 'Infini Italic',    ReadFile('./fonts/infini/infini-italique.otf')), 
#  ( 7, 'Lobster',          ReadFile('./fonts/lobster/Lobster.otf')), 
#  ( 8, 'Odin Bold Italic', ReadFile('./fonts/odin-rounded/Odin Rounded - Bold Italic.otf')), 
#  ( 9, 'Odin Bold',        ReadFile('./fonts/odin-rounded/Odin Rounded - Bold.otf')), 
#  (10, 'Pecita',           ReadFile('./fonts/pecita/Pecita.otf')), 
#  (11, 'Prida',            ReadFile('./fonts/prida65/Prida65.otf'));
#EOF

