#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

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

// Function to handle the client
void* clientHandler(void* arg){
    int clientSocket = *((int*)arg);
    char buffer[1024];
    while(1){
        int data = recv(clientSocket, buffer, sizeof(buffer), 0);
        if(data==-1){
            perror("Error in receiving");
            close(clientSocket);
            pthread_exit(NULL);
        }
        if(data==0){
            printf("Client disconnected\n");
            close(clientSocket);
            pthread_exit(NULL);
        }
        int n = atoi(buffer);
        unsigned long long result = factorial(n);
        char res[1024];
        sprintf(res, "%llu", result);
        if(send(clientSocket, res, sizeof(res), 0)==-1){
            perror("Error in sending");
            close(clientSocket);
            pthread_exit(NULL);
        }
    }
    close(clientSocket);
    printf("Client disconnected\n");
    pthread_exit(NULL);
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error in socket creation");
        exit(1);
    }
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8084); // Port number
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
        if (clientSocket == -1) {
            perror("Error in accepting connection");
            continue; 
        }

        pthread_t tid;
        int x = pthread_create(&tid, NULL, clientHandler, &clientSocket);
        if (x != 0) {
            perror("Error in creating thread");
            close(clientSocket);
            continue;
        }
        pthread_join(tid, NULL);
    }
    close(serverSocket); 
    printf("Server closed\n");
    return 0;
}
