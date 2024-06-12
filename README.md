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
4) Go to File->Preferences->Network in virtual box
5) Click on network and create a new nat network
    - Specify a name for the network
    - Specify an ip range in CIDR notation (eg. 10.0.2.0/24)
    - Set Supports DHCP to true
6) Now for both the server and client vm's go to the VM settings and under Network set the **Attached To** field to **NAT Network** and *Network Name* to the name previously assigned
7) Now proceed to install the OS on the VM's.

## Server & Client Settings/Usage

1) Start Client and Server VM's and run `ulimit -n 8192` to increase the max number of files that can be opened by a process at a time.
2) untar the clients.xz file by running the command `tar -xvf clients.xz`
3) Copy the client binary to your *Client VM* as per the target architecture
4) When testing, run the client binary inside the Client VM as follows 
    - For x86_64   : `./client_x86_64 <ip:port> <num_requests> <num_concurrent_connections>` 
    - For Aarch 64 : `./client_aarch64 <ip:port> <num_requests> <num_concurrent_connections>`
5) ip:port - Ip and port of the server (Eg. 10.0.2.5:8081)
   num_request - Total number of requests to send per connection.
   num_concurrent_connections - Total number of concurrent connections to be maintained between client and server

