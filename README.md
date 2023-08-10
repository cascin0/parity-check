# parity-check
This is simple demo to better understand what parity check is and how it works in the context of network system.

# How to run
Compile using `./build.sh`.

Start the server with `build/server 8080`, where the first arguement is the listening port.

Start the client with `build/client 127.0.0.1 8080`, where the first argument is the IP address of the server and the second one the listening port.

As soon as you run the client, it will start sending 256 numbers to the server. The most significant bit of all of them is going to be the parity bit.

The first half won't be altered, while in the second one some of them will have one bit flipped.

What you should see is the server acknowledging the correctness of the first half and detecting the errors in a portion of the numbers of the second one.
Something like:
```
received: 117, num: 117, OK odd parity
received: 118, num: 118, OK odd parity
received: 247, num: 119, OK odd parity
received: 252, num: 124, ERROR even parity
received: 125, num: 125, ERROR even parity
```
Which tells:
1. The raw `uint8_t` the server received, including the parity bit
2. The number the server received, excluding the parity bit
3. If the parity of the raw `uint8_t` is the expected one (odd in this case)
