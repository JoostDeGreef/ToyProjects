#!/bin/bash

echo GoogleTest
(cd GoogleTest;./update.sh) || exit 1

echo SQLite
(cd SQLite;./update.sh) || exit 1

echo GLFW
(cd GLFW;./update.sh) || exit 1

echo GLAD
(cd GLAD;./update.sh) || exit 1

echo boost
(cd boost;./update.sh) || exit 1

echo freetype
(cd freetype;./update.sh) || exit 1

echo PNG
(cd PNG;./update.sh) || exit 1

echo ZLIB
(cd ZLIB;./update.sh) || exit 1

echo fmt
(cd fmt;./update.sh) || exit 1

