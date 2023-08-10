#!/bin/sh

mkdir -p build

clang src/server.c -o build/server
clang src/client.c -o build/client
