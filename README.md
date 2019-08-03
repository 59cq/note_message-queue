# note_message-queue
学习笔记：操作系统-消息队列
### 消息队列
消息通信方式以消息缓冲区为中间介质，通信双方的发送和接收操作均以消息为单位。在存储器中，消息缓冲区被组织成队列，通常称之为消息队列。由内核管理,可以按顺序发送消息包(消息类型+消息内容),可以全双工工作

- `int msgget(key_t key, int msgflg);`
  - 创建/获取消息队列
  - key IPC键值，由ftok函数生成
  - msgflg
    - 0 获取消息队列
    - IPC_CREAT 创建队列
    - IPC_EXCL 如果存在则创建失败
  - 返回值 消息队列标识符

- `int msgsnd(int msqid, const void *msgp, size_t msgs_z, int msgflg);`
  - 向消息队列发送消息
  - msqid 消息队列标识符msgget函数的返回值
  - msgp 结构指针
    ```C
    struct msgbuf {
        long mtype;       /* message type, must be > 0 */
        char mtext[1];    /* message data */
    };
    ```
   - 消息的长度,不包括消息类型,`sizeof(msgbuf)-4`
   - msgflg
     - 0 阻塞,当消息队列满时等待
     - IPC_NOWAIT 不阻塞,不等待
   - 成功0 失败-1

- `ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);`
  - 从消息队列获取消息
  - msqid 消息队列标识符msgget函数的返回值
  - msgp 结构指针
  - msgsz 要接收消息的长度，可以长一些
  - msgtyp 要接收消息的类型
    - 0 接收任意类型的消息(第一个)
    - \> 0 只接收msgtyp类型的消息
    - < 0 接收消息队列中小于等于msgtyp绝对值的消息,取最小的哪个
  - msgflg 
    - 0 阻塞,消息队列中是否有对应类型的消息,没有则等待
    - 1 不阻塞,消息队列中没有对用类型的消息,则返回
    - 当消息类型正确，而消息的实际长度大于msgsz，则不接收消息且返回-1
    - MSG_NOERROR 把多余的消息截去，成功接收
    - IPC_NOWAIT 如果消息队列没有要接收的数据,则不等待直接返回
    - MSG_EXCEPT 接收消息队列中第一个不是msgtyp的消息,需要编译时加-D_GNU_SUORCE 参数

- `int msgctl(int msqid, int cmd, struct msqid_ds *buf));`
  - 删除消息队列,设置或获取消息队列的属性
  - msqid 消息队列标识符msgget函数的返回值
  - cmd
    - IPC_STAT 获取消息队列的属性
    - IPC_SET 设置消息队列的属性
    - IPC_RMID 删除消息队列
  - buf

    ```c
    struct msqid_ds {
        struct ipc_perm msg_perm;     /* Ownership and permissions */
        time_t          msg_stime;    /* Time of last msgsnd(2) */
        time_t          msg_rtime;    /* Time of last msgrcv(2) */
        time_t          msg_ctime;    /* Time of last change */
        unsigned long   __msg_cbytes; /* Current number of bytes in
                                                    queue (nonstandard) */
        msgqnum_t       msg_qnum;     /* Current number of messages
                                                    in queue */
        msglen_t        msg_qbytes;   /* Maximum number of bytes
                                                    allowed in queue */
        pid_t           msg_lspid;    /* PID of last msgsnd(2) */
        pid_t           msg_lrpid;    /* PID of last msgrcv(2) */
            };
    ```
  - 返回值 成功0 失败-1

简单例程:
```
Terminal1:

gcc send.c -o send
./send

Terminal2:
gcc receive.c -o receive
./receive
```


