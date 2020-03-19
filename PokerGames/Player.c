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
    char desk[5][5];
    strcpy(desk[1],argv[1]);
    strcpy(desk[2],argv[2]);
    strcpy(desk[3],argv[3]);
    strcpy(desk[4],argv[4]);
    //printf("here\n");
    int score;
    int i,j,num;
    int card[5];
    int keep=0;
    int draw;
    int end=0;
    char message[10];
    char buf[10];
    int len;
    int n;
    while(end!=1)
    {
        strcpy(buf,"");
        strcpy(message,"");
        keep=0;
        n=read(STDIN_FILENO,buf,10);
        buf[n]='\0';
        //fprintf(stderr,"buf=%s\n",buf);
        if(strcmp(buf,"WIN\n")==0)
        {
            fprintf(stderr,"buf=%s\n",buf);
            //fprintf(stderr,"here\n");
            //getchar();
            return 0;
        }
        score=atoi(buf);
        for(i=1;i<=4;i++)
        {
            if(strcmp(desk[i],"A")==0) card[i]=1;
            else if(strcmp(desk[i],"4")==0) card[i]=0;
            else if(strcmp(desk[i],"J")==0) card[i]=0;
            else if(strcmp(desk[i],"10")==0) card[i]=0;
            else if(strcmp(desk[i],"Q")==0) card[i]=0;
            else if(strcmp(desk[i],"K")==0) card[i]=0;
            else
            {   card[i]=atoi(desk[i]);
            }
            //fprintf(stderr,"%s\n",desk[i]);
            //fprintf(stderr,"%d\n",card[i]);
        }
        //for(i=1;i<5;i++)    fprintf(stderr,"%s ",desk[i]);
        //fprintf(stderr,"\n");
        strcpy(buf,"");
        for(i=1;i<=4;i++)
        {
            if((score+card[i])<=99)
            {
                if(strcmp(desk[i],"10")==0||strcmp(desk[i],"Q")==0)
                    len=sprintf(buf,"%c %s\n",'-',desk[i]);
                else
                    len=sprintf(buf,"%c %s\n",'+',desk[i]);
                write(STDOUT_FILENO,buf,len);
                keep=1; //note that whether you are alive
                draw=i; //remember which card you draw
                break;
            }
        }
        if(keep==0)//draw a card to lose
        {
            len=sprintf(buf,"%c %s\n",'+',desk[1]);
            write(STDOUT_FILENO,buf,len);
            draw=1;
        }
        n=read(STDIN_FILENO,message,10);
        message[n-1]='\0';
        //fprintf(stderr,"message=%s\n",message);
        if(strcmp(message,"FAIL")!=0){
            n=sprintf(desk[draw],"%s",message);
            strcpy(message,"");
            strcpy(message,"ACK\n");
            write(STDOUT_FILENO,message,4);
        }
        else    end=1;
    }
    return 0;
}
