#!/bin/sh
set -e

CXX="g++"

run() {
  echo ">" "$@"
  "$@"
}

echo "Building libnumber.a"
run $CXX -fPIC -c Number.cpp -o Number.o
run ar rcs libnumber.a Number.o

echo "Building libvector.so"
run $CXX -fPIC -shared Vector.cpp -L. -lnumber -o libvector.so

echo "Building main"
run $CXX main.cpp -L. -lvector -lnumber -Wl,-rpath='$ORIGIN' -o main

echo "Running main"
./main

echo
echo "Linked libraries:"
ldd ./main | grep vector