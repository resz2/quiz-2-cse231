#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct msgBuff  {
    long msgType;
    char msgText[25];
};

int main()  {
    struct msgBuff message;
    key_t key;
    int msgid;

    FILE *f = fopen("para2.txt", "r");
    if(!f)  {
        perror("file does not exist");
        return 1;
    }

    key = ftok("keyfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.msgType = 1;

    while(fscanf(f, "%s", message.msgText)!=EOF) {
        msgsnd(msgid, &message, sizeof(message.msgText), 0);
    }

    fclose(f);
    return 0;
}