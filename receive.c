#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sreing.h>
#include <sts/msg.h>
#include "struct.h"
int main(){
    //获取消息队列
    int msgid = msgget(ftok(".",119),0);
    if(0 > msgid){
        perror("msgget");
        return -1;
    }
    Msg msg = {};
    for(;;){
        msgrcv(msgid,&msg,sizeof(Msg),555,0);
        printf("read:%s\n",msg.data);
        if(0 == strcmp("quit",msg.data)){
            printf("通信结束\n");
            break;
        }
    }
}


