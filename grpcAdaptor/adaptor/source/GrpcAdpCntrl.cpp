/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2004-2020                                           */
/* Mavenir Systems, Inc.                                             */
/* Richardson, TX, USA                                               */
/*                                                                   */
/* ALL RIGHTS RESERVED                                               */
/*                                                                   */
/* Permission is hereby granted to licensees of Mavenir Systems,     */
/* Inc. products to use or abstract this computer program for the    */
/* sole purpose of implementing a product based on                   */
/* Mavenir Systems, Inc. products.  No other rights to reproduce,    */
/* use, or disseminate this computer program, whether in part or in  */
/* whole, are granted.                                               */
/*                                                                   */
/* Mavenir Systems, Inc. makes no representation or warranties       */
/* with respect to the performance of this computer program, and     */
/* specifically disclaims any responsibility for any damages,        */
/* special or consequential, connected with the use of this program. */
/*                                                                   */
/*********************************************************************/

/**********************************************************************
*
* File Name: CUUPLibCntrl.cpp
*
* Subsystem: libCUUP
*
* Description: This file defines the RPC client for libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher       Initial Release
*
**********************************************************************/
#include "GrpcAdpCntrl.h"
#include "GrpcAdpThreads.h"
#include "SvcLog.h"
#include "ServiceCommonType.h"


namespace mav
{
    GrpcAdpCntrl* GrpcAdpCntrl::mInstance = nullptr;
    int GrpcAdpCntrl::mInstId = 0;

    GrpcAdpCntrl::GrpcAdpCntrl():
        mThreadNum(1),
        mThreadInstanceIdGen(1)
    {
        
    }
   
    GrpcAdpCntrl* GrpcAdpCntrl::getInstance()
    {
        if (!mInstance)
        {
            mInstance = new GrpcAdpCntrl();
        }
        return mInstance;
    }
   
    int GrpcAdpCntrl::initGrpcServerConfig()
    {
        //get the grpc port config from  
        std::string cucpGrpcIP = "0.0.0.0";
        std::string cucpGrpcPort = "9021";
        getGrpcServerThread().serverAddr() = cucpGrpcIP + ":" + cucpGrpcPort;

        printf("CUCP ip : %s Port: %d ", cucpGrpcIP.c_str(), cucpGrpcPort.c_str());
        ccLog("CUCP ip : %s Port: %d ", cucpGrpcIP.c_str(), cucpGrpcPort.c_str());
        ccLog("init grpc server config: %s ", getGrpcServerThread().serverAddr());
        return 0;

    }

    int GrpcAdpCntrl::initGrpcClientConfig()
    {
        //get the grpc port config from  
        std::string ebmGrpcIP = "0.0.0.0";
        std::string ebmGrpcPort = "9027";
        getGrpcClientThread().ebmServerTarget() = ebmGrpcIP + ":" + ebmGrpcPort;
        printf("CUCP ip : %s Port: %d ", ebmGrpcIP.c_str(), ebmGrpcPort.c_str());
        //init the mutex
        if (pthread_mutex_init(&this->mClientMutex, NULL) != 0){
            ccLog(" client init mutex failed");
        }

        return 0;

    }

    int GrpcAdpCntrl::startGrpcServerThread(unsigned char& aThreadInstanceId)
    {
        unsigned char threadInstanceId = mThreadInstanceIdGen++;

        mRpcServerThread.init(threadInstanceId);
        aThreadInstanceId = threadInstanceId;
        mRpcServerThread.run();
        return 0;
    }


    int GrpcAdpCntrl::startGrpcClientThread(unsigned char& aThreadInstanceId)
    {
        unsigned char threadInstanceId = mThreadInstanceIdGen++;
        
        mRpcClientThread[0]->init(threadInstanceId);
        aThreadInstanceId = threadInstanceId;
        ccLog("startGrpcClientThread  threadInstanceId %x ", threadInstanceId);
        mRpcClientThread[0]->run();
        return 0;
    }

    int GrpcAdpCntrl::createGrpcClient(string cuCpID, long cuCpInst, string cuUpID)
    {
        EbmGrpcClient record {cuCpID, cuCpInst, cuUpID};
        GrpcAdpRpcClientThread *grpcClient = new GrpcAdpRpcClientThread();
        grpcClient->set_ebm_ip_port(cuUpID);
        grpcClient->run();
        mEbmClientMap.insert(pair<EbmGrpcClient,int>(record, 0));;
    }

    int GrpcAdpCntrl::destroyGrpcClient(string cuCpID, long cuCpInst, string cuUpID)
    {
        EbmGrpcClient client {cuCpID, cuCpInst, cuUpID};
        mEbmClientMap.erase(client);
    }

    int GrpcAdpCntrl::createWorkerThreads()
    {
        #if 0
        int ret;
        pthread_t th;
        int arg = 10;
        mWorkerInstance.push_back(nullptr);
        for(int i = 1; i <= 2; i++)
        {
            unsigned char threadInstancedId = mThreadInstanceIdGen;
            mThreadInstanceIdGen++;
            GrpcWorker *theWorker = new GrpcWorker(threadInstancedId);
            printf("worker add %p", theWorker);

            printf("create worker %d ", threadInstancedId);
            ret = pthread_create( &th, NULL, grpcServer, &arg);
            if (ret != 0){
                printf("Create thread error!\n");
                return -1;
            }
        }
        #endif
        return 0;
    }

    bool GrpcAdpCntrl::init()
    {
        unsigned char threadNum = mThreadInstanceIdGen;

        for (int idx = 0; idx < MAX_GRPC_CLIENT_NUM; idx ++) {
            mRpcClientThread[idx] = nullptr;
        }
        
        createWorkerThreads();

        initGrpcServerConfig();

        startGrpcServerThread(threadNum);
        ccLog("Start RPC Server thread %d ", threadNum);

        //initGrpcClientConfig();

        //startGrpcClientThread(threadNum);
        
        ccLog("Start RPC client thread %d ", threadNum);

        return true;
    }

    bool GrpcAdpCntrl::deinit()
    {
        GrpcAdpThreads::getInstance()->killRpcServerThread();
        GrpcAdpThreads::getInstance()->killRpcClientThread();
        if (pthread_mutex_destroy(&this->mClientMutex) != 0){
            ccLog(" client deinit mutex failed");
        }
        return true;
    }

    void GrpcAdpCntrl::join_threads()
    {
        GrpcAdpThreads::getInstance()->joinRpcServerThread();
    }

    void GrpcAdpCntrl::client_lock(int clientId)
    {
        if (pthread_mutex_lock(&this->mClientMutex) != 0){
            ccLog(" client_lock failed client %d", clientId);
        }
        return;
    }

    void GrpcAdpCntrl::client_unlock(int clientId)
    {
        if (pthread_mutex_unlock(&this->mClientMutex) != 0){
            ccLog(" client_unlock failed client %d", clientId);
        }
        return;
    }

    int GrpcAdpCntrl::create_client(char* cuCpID, long cuCpInst, char* cuUpID)
    {
        string cpID = cuCpID;
        string upID = cuUpID;
        return createGrpcClient(cpID, cuCpInst, upID);
    }

    int GrpcAdpCntrl::get_free_client_idx(GrpcAdpRpcClientThread* threadID)
    {
        int idx;
        int ret = MAX_GRPC_CLIENT_NUM;
        client_lock(0);
        for (idx = 0; idx < MAX_GRPC_CLIENT_NUM; idx++) {
            if (mRpcClientThread[idx] == nullptr) {
                ret = idx;
                mRpcClientThread[idx] = threadID;
                break;
            }
        }
        client_unlock(0);
        return ret;
    }

    int GrpcAdpCntrl::release_client_idx(int index)
    {

        client_lock(0);
        mRpcClientThread[index] = nullptr; 
        client_unlock(0);
        return 0;
    }


} // namespace mav

