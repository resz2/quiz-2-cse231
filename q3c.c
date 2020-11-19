#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

int main(void){
    char buff[500];
    int socketDesc;
    struct sockaddr_in serverAddress, clientAddress;

    socketDesc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socketDesc<0){
        perror("Error while creating socket");
        return -1;
    }
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(socketDesc, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }

    int n, clientLength = sizeof(clientAddress);
    while(n = recvfrom(socketDesc, (char *)buff, sizeof(buff), MSG_WAITALL, (struct sockaddr*)&clientAddress, &clientLength)!=0){
        buff[n] = '\0';
        printf("%s ", buff);
    }

    close(socketDesc);
    return 0;
}