#!/bin/sh

mkdir -p build

gcc src/socket.c -c -o build/socket.o
gcc src/parity.c -c -o build/parity.o
gcc src/server.c build/socket.o build/parity.o -o build/server
gcc src/client.c build/parity.o -o build/client
