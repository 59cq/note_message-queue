#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sreing.h>
#include <sts/msg.h>
#include "struct.h"
int main(){
    //创建消息队列
    int msgid = msgget(ftok(".",119),IPC_CREAT|IPC_EXCL|0644);
    if(0 > msgid){
        perror(msgget);
        return -1;
    }
    Msg msg = {666};
    for(;;){
        printf(">");
        gets(msg.data);
        msgsnd(msgid,&msg,sizeof(Msg)-sizeof(msg.type),0);
        if(0 == strcmp("quit",msg.data)){
            printf("通信结束\n");
            break;
        } 
    }
    //删除消息队列
    if(0 > msgctl(msgid,IPC_RMID,NULL)){
        perror("msgctl");
        return -1;
    }
}



