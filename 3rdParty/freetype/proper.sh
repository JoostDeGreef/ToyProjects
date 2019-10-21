#!/bin/bash

rm -rf download
rm -rf freetype*
rm -rf tmp
rm -rf ../include/freetype

find . -name "*~" -exec rm -f "{}" \;

