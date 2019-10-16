#!/bin/bash

mkdir -p out
(cd 3rdParty;./update.sh)
(cd out;cmake ..;make)
