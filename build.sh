#!/bin/sh

mkdir -p build

gcc src/server.c -o build/server
gcc src/client.c -o build/client
