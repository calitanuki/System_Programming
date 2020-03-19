#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <signal.h>
int main(int argc,char* argv[])
{
    int len;
    char mes[25];
    unlink(argv[1]);
    mkfifo(argv[1],0666);
    int fifo_fd=open(argv[1],O_RDWR|O_NONBLOCK);
    int fd=open("Result.txt",O_WRONLY|O_CREAT);
    while(1)
    {
        //strcpy(mes,"");
        len=read(fifo_fd,mes,25);
        write(fd,mes,len);
        //mes[len]='\0';
    }
    close(fifo_fd);
    close(fd);
    return 0;
}
