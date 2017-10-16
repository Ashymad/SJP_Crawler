#!/bin/sh

# Exit on first error
set -e

mkdir -p build
cd build

# Generate a Makefile for GCC (or Clang, depanding on CC/CXX envvar)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build (ie 'make')
cmake --build .
