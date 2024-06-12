# Analysing-I-O-Multiplexing-Techniques
Analysis of synchronous &amp; asynchronous I/O multiplexing techniques like select() & poll() system call, epoll API(non-blocking) and fork, pthread(blocking) using seperate TCP Client/Server programs. Captured TCP packets(pcaps) communicated between the client and the server on the server’s interface using tcpdump and analysed them using python scripts for monitoring average throughput/latency as well as server process’s CPU & memory utilization.

TCP Server Programs:-
1. Select() system call
2. poll() system call\
3. epoll API
4. fork()
5. pthread()

Server Function:-
a. Read the payload and cast it to a 64-bit unsigned integer, n.
b. Write a function, fact(n), to compute the factorial of the “n”; if n>20, then calculate
the factorial of 20.
c. Send the factorial result as the response message back to the client.

Client Function:-
50/100/500/1000 concurrent client connections are formed each of which generate 50 requests to the server at the same time.
