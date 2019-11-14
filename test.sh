#!/bin/bash

mkdir -p out
(cd out;cmake ..)

make -C out LoggerTest   && LD_LIBRARY_PATH=out/bin out/bin/LoggerTest
make -C out UtilsTest    && LD_LIBRARY_PATH=out/bin out/bin/UtilsTest
make -C out MathCoreTest && LD_LIBRARY_PATH=out/bin out/bin/MathCoreTest
make -C out 3rdPartyTest && LD_LIBRARY_PATH=out/bin out/bin/3rdPartyTest
make -C out DataTest     && LD_LIBRARY_PATH=out/bin out/bin/DataTest

