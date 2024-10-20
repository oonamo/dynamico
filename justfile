set shell := ["pwsh.exe", "-Command"]

build:
  clang -o test tests/dynamico.c -O3

test: build
  ./test

debug:
  clang -o test tests/dynamico.c -g -Wall
  clang -o generic tests/generic.c -g -Wall
