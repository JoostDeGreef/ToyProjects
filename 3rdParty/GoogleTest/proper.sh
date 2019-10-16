#!/bin/bash

rm -rf googletest 
rm -rf src
rm -rf tmp
rm -f src-extra/gtest-all.cc

find . -name "*~" -exec rm -f "{}" \;

