# Analysing-I-O-Multiplexing-Techniques
Analysis of synchronous &amp; asynchronous I/O multiplexing techniques like select() & poll() system call, epoll API(non-blocking) and fork, pthread(blocking) using seperate TCP Client/Server programs. Captured TCP packets(pcaps) communicated between the client and the server on the server’s interface using tcpdump and analysed them using python scripts for monitoring average throughput/latency as well as server process’s CPU & memory utilization.

## TCP Server Programs:-
1. Select() system call
2. poll() system call\
3. epoll API
4. fork()
5. pthread()

## Server Function:-  
1. Read the payload and cast it to a 64-bit unsigned integer, n.
2. Write a function, fact(n), to compute the factorial of the “n”; if n>20, then calculate
the factorial of 20.  
3. Send the factorial result as the response message back to the client.    

## Client Function:-  
1) 50/100/500/1000 concurrent client connections are formed.  
2) Each of which generate 50 requests to the server at the same time.  

## How to setup your system?

1) You will require two VM's - One for running the client and one for running the server (Both the VM's should be able to communicate with each other).
2) Client Specs  
    - Memory  : 2048MB  
    - Disk    : 10GB- 15GB (For Guest OS)  
    - CPU     : 2cores  
    - OS      : Ubuntu 22.04 + / Debian 11+ (GLIBC VERSION - 2.31+)
3) Server Specs  
    - Memory  : 2048MB  
    - Disk    : 10GB - 15GB (For Guest OS)  
    - CPU     : 3cores  
    - OS      : Any version of ubuntu/debian  
