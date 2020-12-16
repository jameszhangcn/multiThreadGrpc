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
        
        mRpcClientThread.init(threadInstanceId);
        aThreadInstanceId = threadInstanceId;
        ccLog("startGrpcClientThread  threadInstanceId %x ", threadInstanceId);
        mRpcClientThread.run();
        return 0;
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
        
        createWorkerThreads();

        initGrpcServerConfig();

        startGrpcServerThread(threadNum);
        ccLog("Start RPC Server thread %d ", threadNum);

        initGrpcClientConfig();

        startGrpcClientThread(threadNum);
        
        ccLog("Start RPC client thread %d ", threadNum);

        return true;
    }

    bool GrpcAdpCntrl::deinit()
    {
        GrpcAdpThreads::getInstance()->killRpcServerThread();
        GrpcAdpThreads::getInstance()->killRpcClientThread();
        return true;
    }

    void GrpcAdpCntrl::join_threads()
    {
        GrpcAdpThreads::getInstance()->joinRpcServerThread();
    }
} // namespace mav

