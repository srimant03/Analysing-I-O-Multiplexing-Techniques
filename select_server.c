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
    serverAddress.sin_port = htons(8085); //PORT NUMBER
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

    fd_set readfds,writefds;
    FD_ZERO(&writefds);
    FD_SET(serverSocket,&writefds);
    int max_sd;
    int client_Socket[4000];
    for(int i=0;i<4000;i++){
        client_Socket[i]=0;
    }
    while(1){
        FD_ZERO(&readfds);
        FD_SET(serverSocket,&readfds);
        max_sd=serverSocket;
        for(int i=0;i<4000;i++){
            if(client_Socket[i]>0){
                FD_SET(client_Socket[i],&readfds);
            }
            if(client_Socket[i]>max_sd){
                max_sd=client_Socket[i];
            }
        }
        int activity = select(max_sd+1,&readfds,&writefds,NULL,NULL);
        if(activity<0){
            perror("Error in select");
            exit(1);
        }
        if(FD_ISSET(serverSocket,&readfds)){
            struct sockaddr_in clientAddress;
            socklen_t clientAddressLength = sizeof(clientAddress);
            printf("Waiting for a client to connect...\n");
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
            printf("Client connected\n");
            if (clientSocket == -1) {
                perror("Error in accepting connection");
                continue; 
            }
            for(int i=0;i<4000;i++){
                if(client_Socket[i]==0){
                    client_Socket[i]=clientSocket;
                    break;
                }
            }
        }
        for(int i=0;i<4000;i++){
            if(FD_ISSET(client_Socket[i],&readfds)){
                char buffer[1024];
                int data = recv(client_Socket[i], buffer, sizeof(buffer), 0);
                if(data==-1){
                    perror("Error in receiving");
                    close(client_Socket[i]);
                    client_Socket[i]=0;
                    continue;
                }
                if(data==0){
                    printf("Client disconnected\n");
                    close(client_Socket[i]);
                    client_Socket[i]=0;
                    continue;
                }
                int n = atoi(buffer);
                unsigned long long result = factorial(n);
                char res[1024];
                sprintf(res, "%llu", result);
                if(send(client_Socket[i], res, sizeof(res), 0)==-1){
                    perror("Error in sending");
                    close(client_Socket[i]);
                    client_Socket[i]=0;
                    continue;
                }
            }
        }
    }
    close(serverSocket);
    printf("Server socket closed\n");
    return 0;
}