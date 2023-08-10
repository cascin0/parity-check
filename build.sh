#!/bin/sh

mkdir -p build

gcc src/parity.c -c -o build/parity.o
gcc src/server.c build/parity.o -o build/server
gcc src/client.c build/parity.o -o build/client
