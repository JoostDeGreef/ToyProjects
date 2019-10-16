#!/bin/bash

rm -rf out
mkdir out
(cd 3rdParty;./update.sh)
(cd out;cmake ..;make)



