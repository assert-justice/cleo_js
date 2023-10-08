#!/usr/bin/env bash

mkdir dist
cp ./bin/cleo ./dist && butler push ./dist etmm/cleo:linux
rm -rf dist