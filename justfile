set shell := ["pwsh.exe", "-Command"]

build:
  gcc -o test tests/dynamico.c -O3

test: build
  ./test
