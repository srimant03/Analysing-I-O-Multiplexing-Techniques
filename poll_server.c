#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>     
#include <arpa/inet.h>   
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> 
#include <poll.h>

// Function to calculate the factorial
unsigned long long factorial(int n){
    if(n==0 || n==1){
        return 1;
    }
    if(n>=20){
        return 2432902008176640000;
    }
    unsigned long long fact[n+1];
    fact[0]=1;
    fact[1]=1;
    for(int i=2;i<=n;i++){
        fact[i]=i*fact[i-1];
    }
    return fact[n];
}

int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error in socket creation");
        exit(1);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8086); //PORT NUMBER
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error in binding");
        close(serverSocket);
        exit(1);
    }

    if (listen(serverSocket, 4000) == -1) {
        perror("Error in listening");
        close(serverSocket);
        exit(1);
    }
    printf("Server is listening...\n");

    struct pollfd fds[4001];
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;
    int max_sd = 0;
    int clientSocket[4000];
    for(int i=0;i<4000;i++){
        clientSocket[i]=0;
    }

    while(1){
        int poll_status = poll(fds, max_sd+1, -1);
        if(poll_status==-1){
            perror("Error in polling");
            exit(1);
        }
        if(poll_status==0){
            continue;
        }
        if(fds[0].revents & POLLIN){
            struct sockaddr_in clientAddress;
            socklen_t clientAddressLength = sizeof(clientAddress);
            printf("Waiting for a client to connect...\n");
            int newSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
            printf("Client connected\n");
            if (newSocket == -1) {
                perror("Error in accepting connection");
                continue; 
            }
            for(int i=0;i<4000;i++){
                if(clientSocket[i]==0){
                    clientSocket[i]=newSocket;
                    fds[i+1].fd = newSocket;
                    fds[i+1].events = POLLIN;
                    break;
                }
            }
            if(newSocket>max_sd){
                max_sd=newSocket;
            }
        }
        for(int i=1;i<=max_sd;i++){
            if(fds[i].revents & POLLIN){
                char buffer[1024];
                int data = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if(data==-1){
                    perror("Error in receiving");
                    close(fds[i].fd);
                    fds[i].fd=-1;
                    clientSocket[i-1]=0;
                    continue;
                }
                if(data==0){
                    printf("Client disconnected\n");
                    close(fds[i].fd);
                    fds[i].fd=-1;
                    clientSocket[i-1]=0;
                    continue;
                }
                int n = atoi(buffer);
                unsigned long long result = factorial(n);
                char res[1024];
                sprintf(res, "%llu", result);
                if(send(fds[i].fd, res, sizeof(res), 0)==-1){
                    perror("Error in sending");
                    close(fds[i].fd);
                    fds[i].fd=-1;
                    clientSocket[i-1]=0;
                    continue;
                }
            }
        }
    }
    close(serverSocket);

    return 0;
}

