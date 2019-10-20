#!/bin/bash

(cd GoogleTest;./update.sh) || exit 1
(cd SQLite;./update.sh) || exit 1
(cd GLFW;./update.sh) || exit 1
(cd GLAD;./update.sh) || exit 1
(cd boost;./update.sh) || exit 1
(cd freetype;./update.sh) || exit 1
(cd PNG;./update.sh) || exit 1
(cd ZLIB;./update.sh) || exit 1

