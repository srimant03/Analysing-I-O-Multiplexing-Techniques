#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

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

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error in socket creation");
        exit(1);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8083); //PORT NUMBER
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
    while (1) {
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        printf("Waiting for a client to connect...\n");
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        printf("Client connected\n");
        if (clientSocket == -1) {
            perror("Error in accepting connection");
            continue; 
        }
        int childPid = fork();
        if (childPid == 0) {
            close(serverSocket); 
            char buffer[1024];
            while (1) {
                int messageSize = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (messageSize == -1) {
                    perror("Error in receiving message");
                    break;
                }
                if (messageSize == 0) {
                    printf("Client disconnected\n");
                    break;
                }
                //printf("Received message: %s\n", buffer);
                // Send the message back to client
                //printf("Sending message back to client\n");
                unsigned long long fact = factorial(atoi(buffer));
                sprintf(buffer, "%llu", fact);
                if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
                    perror("Error in sending message");
                    break;
                }
            }
            close(clientSocket); 
            printf("Client socket closed\n");
            exit(0); 
        } else if (childPid < 0) {
            perror("Error in forking");
        } else {
            close(clientSocket); 
        }
    }

    close(serverSocket); 
    return 0;
}
