#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

int main(void){
    int socketDesc;
    struct sockaddr_in serverAddress;
    char serverMsg[25];

    FILE *f = fopen("para2.txt", "r");
    if(!f)  {
        perror("file does not exist");
        return 1;
    }

    socketDesc = socket(AF_INET, SOCK_DGRAM, 0);
    memset(serverMsg, '\0', sizeof(serverMsg));
    
    if(socketDesc < 0){
        perror("Error while creating socket");
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    while(fscanf(f, "%s", serverMsg)!=EOF) {
        printf("%s ", serverMsg);
        if(sendto(socketDesc, serverMsg, strlen(serverMsg), MSG_CONFIRM, (struct sockaddr*)&serverAddress, sizeof(serverAddress))<0) {
            perror("Unable to send message");
            return -1;
        }
    }

    fclose(f);
    close(socketDesc);
    return 0;
}