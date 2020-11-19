#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct msgBuff {
    long msgType;
    char msgText[25];
};

int main()  {
    struct msgBuff message;
    key_t key;
    int msgid;

    key = ftok("keyfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    while(msgrcv(msgid, &message, sizeof(message.msgText), 1, IPC_NOWAIT)!=-1)   {
        printf("%s ", message.msgText);
    }
    printf("\n");

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}