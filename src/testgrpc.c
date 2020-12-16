#include "pthread.h"
#include <stdio.h>
#include <unistd.h>
#include "../include/GrpcAdp.h"

void *grpcClient(void *arg){
    int ret;
    pid_t pid;
    pthread_t tid;
    printf("This is grpc client thread!");
    *(int*)arg = 0;
    CuCpUpHdrG cp_up_hdr;
    SecurityConfigureG secCfg;
    secCfg.algorithm_type = 1234;
    for (int i=0; i < CPJ_CIPHER_KEY_LEN_G; i++) {
        //aMsg.mutable_security_config()->set_encryption_key(i,secCfg->encryption_key[i]);
        secCfg.encryption_key[i] = i;

    }

    pid = getpid();
    tid = pthread_self();

    while(1){
        sleep(1);
        printf("In client thread pid %x tid %x \n", pid, tid);
        //send msg to EBM
        ret =  send_sec_cfg_req(&cp_up_hdr, &secCfg);
        printf("send to EBM ret %d pid %x tid %x \n", ret, pid, tid);
    }
    return arg;
}

#define MAX_CLIENT_THREAD_NUM (10)

int main(int argc, char *argv[])
{
    pthread_t th[MAX_CLIENT_THREAD_NUM];
    int ret;
    int idx;
    int arg = 10;
    int *thread_ret[MAX_CLIENT_THREAD_NUM] = {NULL};

    printf("This is the main process.\n");
    start_grpc_server();
    
    for (idx = 0; idx < MAX_CLIENT_THREAD_NUM; idx++) {
        ret = pthread_create( &th[idx], NULL, grpcClient, &arg);
        if (ret != 0){
            printf("Create grpcClient thread error!\n");
            return -1;
        }
    }

    for (idx = 0; idx < MAX_CLIENT_THREAD_NUM; idx++) {
        pthread_join(th[idx], (void**)&thread_ret[idx]);
        printf("thread_ret = %d.\n", * thread_ret[idx]);
    }


    return 0;
}
