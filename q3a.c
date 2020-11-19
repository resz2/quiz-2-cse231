#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

int main(void){
    int socketDesc;
    struct sockaddr_in serverAddress;
    char buff[25];
    char *arr[100];

    FILE *f = fopen("para1.txt", "r");
    if(!f)  {
        perror("file does not exist");
        return 1;
    }

    int i = 0;
    while(fscanf(f, "%s", buff)!=EOF) {
        arr[i] = malloc(25);
        strcpy(arr[i], buff);
        printf("%s ", arr[i]);
        i++;
    }
    fclose(f);

    socketDesc = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&serverAddress, 0, sizeof(serverAddress));
    
    if(socketDesc < 0){
        perror("Error while creating socket");
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    for(int j=0;j<i;j++)   {
        if(sendto(socketDesc, (char*)arr[j], strlen(arr[j]), MSG_CONFIRM, (struct sockaddr*)&serverAddress, sizeof(serverAddress))<0) {
            perror("Unable to send message");
            return -1;
        }
    }

    close(socketDesc);
    return 0;
}