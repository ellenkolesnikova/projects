`server.s` is a webserver written in x86 assembly. It handles multiple simultaneous GET requests from nc commands.

**To run**

In one terminal, create a txt file (e.g. `hello.txt`) and run:

    ./server

In another terminal, run:

    nc localhost 80

Get the txt file's contents using an HTTP GET request, served by the server:

    GET hello.txt HTTP/1.0
