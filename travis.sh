#!/usr/bin/env bash

git submodule update --init --recursive
mkdir b
cd b

# debug
CXX=/usr/bin/g++-6 CC=/usr/bin/gcc-6 cmake -DCMAKE_BUILD_TYPE=DEBUG -DCOVERAGE=1 ../
make
./tests
valgrind --error-exitcode=42 --leak-check=full ./tests

# plain
rm -rf ./*
CXX=/usr/bin/g++-6 CC=/usr/bin/gcc-6 cmake -DCOVERAGE=1 ../
cmake ../
make
./tests
valgrind --error-exitcode=42 --leak-check=full ./tests
