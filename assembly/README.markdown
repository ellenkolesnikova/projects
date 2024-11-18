server.s is a webserver written in x86 assembly. It handles multiple simultaneous GET requests from nc commands.

**To run**

In one terminal, run:

    ./server

In another terminal, run:

    nc localhost 80

You can create a txt file (e.g. `hello.txt`) and get its contents using nc command:

    GET hello.txt HTTP/1.0
