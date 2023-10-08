#!/usr/bin/env bash

cd build
ninja
cd ..
cp build/cleo bin/cleo
cd bin
./cleo
cd ..