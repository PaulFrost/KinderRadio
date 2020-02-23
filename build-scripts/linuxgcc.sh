#!/bin/bash

mkdir -p ../build-KinderRadio-Linux
cd ../build-KinderRadio-Linux
qmake ../KinderRadio.pro -spec linux-g++ CONFIG+=release
make -j4
