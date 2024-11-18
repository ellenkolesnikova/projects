`server.s` is a webserver written in x86 assembly. It handles multiple simultaneous GET requests from nc commands.

**To run**

In one terminal, run:

    ./server

In another terminal, run:

    nc localhost 80

Create a txt file (e.g. `hello.txt`). In the same terminal as the nc command, get the txt file's contents using an HTTP GET request:

    GET hello.txt HTTP/1.0
