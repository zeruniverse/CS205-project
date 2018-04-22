#!/bin/bash

sudo yum install gcc-c++
wget https://cmake.org/files/v3.10/cmake-3.10.0.tar.gz
tar -xvzf cmake-3.10.0.tar.gz
cd cmake-3.10.0
./bootstrap
make
sudo make install
sudo yum install libjpeg-devel
sudo yum install libpng-devel