#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <signal.h>
int fd[4][2][2];
int alive[4]={0};
int main(int argc,char* argv[])
{
    signal(SIGPIPE,SIG_IGN);
    int i,j,k;
    pid_t pid;
    srand(time(NULL));

    for(i=0;i<=3;i++)
    {
        for(j=0;j<2;j++){
            if(pipe(fd[i][j])<0)
                printf("pipe error\n");
        }
        if((pid=fork())<0)
        {
            printf("fork error\n");
        }
        else if(pid>0)//parent
        {
            close(fd[i][0][1]);
            close(fd[i][1][0]);
        }else//child
        {
            close(fd[i][0][0]);
            if(fd[i][0][1]!=STDOUT_FILENO)
            {
                dup2(fd[i][0][1],STDOUT_FILENO);
                close(fd[i][0][1]);
            }
            close(fd[i][1][1]);
            if(fd[i][1][0]!=STDIN_FILENO)
            {
                dup2(fd[i][1][0],STDIN_FILENO);
                close(fd[i][1][0]);
            }
            execl(argv[i+2],argv[i+2],"K","K","K","K",(char*)0);
            _exit(0);
        }
    }
    int id[4];
    for(i=0;i<4;i++)    id[i]=i;
    i=0;
    int card;
    int score=0;
    int back;
    char line[10];
    char sline[10];
    int n;
    int rot=0;
    int playnum=4;
    //int slp = 0;
    while(1)
    {
        strcpy(line,"");   //clear string
        strcpy(sline,"");  //clear string
        /*if(slp == 0){
            sleep(20);
            slp = 1;
        }*/
        //printf("index=%d id now=%d\n",i,id[i]);
        if(playnum==1)
        {
            strcpy(line,"WIN\n");
            n=4;
            write(fd[id[i]][1][1],line,n);
            break;
        }
        n=sprintf(sline,"%d\n",score);
        sline[n]='\0';
        if(write(fd[id[i]][1][1],sline,n)<0)//write score
        {
            alive[i]=1;
            playnum--;
            for(j=i;j<playnum;j++)
            {
                id[j]=id[j+1];
            }
            if(rot==1)
            {
                if(i==0) i=playnum-1;
                else    i--;
            }
            else
            {
                if(i==playnum)  i=0;
            }
            continue;
        }
        strcpy(sline,"");
        n=read(fd[id[i]][0][0],sline,10);//read player choice
        if(n<=0)
        {
            alive[i]=1;
            playnum--;
            for(j=i;j<playnum;j++)
            {
                id[j]=id[j+1];
            }
            if(rot==1)
            {
                if(i==0) i=playnum-1;
                else    i--;
            }
            else
            {
                if(i==playnum)  i=0;
            }
            continue;
        }
        sline[n]='\0';
        if(sline[2]=='1'&&sline[0]=='+')    back=10;
        else if(sline[2]=='1'&&sline[0]=='-')   back=-10;
        else if(sline[2]=='Q'&&sline[0]=='+')   back=20;
        else if(sline[2]=='Q'&&sline[0]=='-')   back=-20;
        else if(sline[2]=='A')   back=1;
        else if(sline[2]=='2')   back=2;
        else if(sline[2]=='3')   back=3;
        else if(sline[2]=='5')   back=5;
        else if(sline[2]=='6')   back=6;
        else if(sline[2]=='7')   back=7;
        else if(sline[2]=='8')   back=8;
        else if(sline[2]=='9')   back=9;
        else if(sline[2]=='K')   score=99;
        else if(sline[2]=='4')
        {
            if(rot==1)  rot=0;
            else    rot=1;
        }

        if(sline[2]!='4'&&sline[2]!='J'&&sline[2]!='K')
            score+=back;
        n=2;
        if(score<=99){
            card=rand()%13+1;
            if(card==1)     strcpy(line,"A\n");
            else if(card==2)     strcpy(line,"2\n");
            else if(card==3)     strcpy(line,"3\n");
            else if(card==4)     strcpy(line,"4\n");
            else if(card==5)     strcpy(line,"5\n");
            else if(card==6)     strcpy(line,"6\n");
            else if(card==7)     strcpy(line,"7\n");
            else if(card==8)     strcpy(line,"8\n");
            else if(card==9)     strcpy(line,"9\n");
            else if(card==11)    strcpy(line,"J\n");
            else if(card==12)    strcpy(line,"Q\n");
            else if(card==13)    strcpy(line,"K\n");
            else if(card==10)
            {
                strcpy(line,"10\n");
                n=3;
            }
            if(write(fd[id[i]][1][1],line,n)<0)//write shuffle
            {
                alive[i]=1;
                playnum--;
                for(j=i;j<playnum;j++)
                {
                    id[j]=id[j+1];
                }
                if(rot==1)
                {
                    if(i==0) i=playnum-1;
                    else    i--;
                }
                else
                {
                    if(i==playnum)  i=0;
                }
                continue;
            }
            if(read(fd[id[i]][0][0],sline,10)<=0)//receive ACK
            {
                alive[i]=1;
                playnum--;
                for(j=i;j<playnum;j++)
                {
                    id[j]=id[j+1];
                }
                if(rot==1)
                {
                    if(i==0) i=playnum-1;
                    else    i--;
                }
                else
                {
                    if(i==playnum)  i=0;
                }
                continue;
            }
            if(rot==1)
            {
                if(i==0) i=playnum-1;
                else    i--;
            }
            else
            {
                if(i==playnum-1) i=0;
                else    i++;
            }
        }
        else
        {
            alive[i]=1;
            playnum--;
            strcpy(line,"FAIL\n");
            write(fd[id[i]][1][1],line,5);
            for(j=i;j<playnum;j++)
            {
                id[j]=id[j+1];
            }
            score-=back;
            if(rot==1)
            {
                if(i==0) i=playnum-1;
                else    i--;
            }
            else
            {
                if(i==playnum)  i=0;
            }
        }
        //for(k=0;k<4;k++) printf("alive[%d]=%d ",k,alive[k]);
        //printf("\n");
        //getchar();
    }
    char fifobuf[10];
    int fifofd=open(argv[6],O_WRONLY); //write to fifo
    n=sprintf(fifobuf,"%s %d\n",argv[1],id[0]+1);
    fifobuf[n]='\0';
    printf("%s",fifobuf);
    write(fifofd,fifobuf,n);
    return 0;
}
