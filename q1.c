#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

void printEscapeChar(char x)    {
    switch(x)   {
        case 92:    printf("\\");
                    break;
        case 39:    printf("\'");
                    break;
        case 34:    printf("\"");
                    break;
        case 116:   printf("\t");
                    break;
        case 110:   printf("\n");
                    break;
        case 118:   printf("\v");
                    break;
        case 97:    printf("\a");
                    break;
        case 98:    printf("\b");
                    break;
        case 102:   printf("\f");
                    break;
        case 114:   printf("\r");
                    break;
        case 63:    printf("\?");
                    break;
        default:    perror("unknown escape sequence");
    }
}

int main()  {
    int fd1[2];
    int fd2[2];
    char input[501];
    pid_t pid;

    if (pipe(fd1)==-1)  {
        perror("Pipe Failed" );
        return 1;
    }
    if (pipe(fd2)==-1)  {
        perror("Pipe Failed" );
        return 1;
    }

    pid = fork();

    if (pid<0)    {
        perror("fork Failed" );
        return 1;
    }

    // Parent process 
    else if(pid>0)   {
        fgets(input, 501, stdin);
        char output[501];

        close(fd1[0]);
        write(fd1[1], input, strlen(input)+1);
        close(fd1[1]);

        wait(NULL);

        close(fd2[1]);
        read(fd2[0], output, 501);
        close(fd2[0]);

        for(int i=0;i<strlen(output);i++)   {
            char x = output[i];
            if(x==92)    {
                printEscapeChar(output[i+1]);
                i += 1;
            }
            else
                printf("%c", x);
        }
    }

    // child process
    else    {
        close(fd1[1]);
        char modified[501];
        read(fd1[0], modified, 501);

        for (int i=0;i<strlen(modified);i++)    {
            if(modified[i]=='\\')   {
                i += 2;
            }
            modified[i] = toupper(modified[i]);
        }

        close(fd1[0]);
        close(fd2[0]);
        write(fd2[1], modified, strlen(modified)+1);
        close(fd2[1]);

        exit(0);
    }
}