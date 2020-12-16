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
* File Name: TrilliumLibCntrl.h
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
#pragma once

#include "CucpService.h"
#include "GrpcAdp.h"
#include "GrpcAdpRpcServerThread.h"
#include "GrpcAdpRpcClientThread.h"

namespace mav
{
        
    class GrpcWorker{
        public:
            GrpcWorker(unsigned char aThreadInstId);
            ~GrpcWorker()
                {
                    //delete context
                }
        private:
            unsigned char mThreadInstId;
    };

    class GrpcAdpCntrl
    {
    public:
        static GrpcAdpCntrl* getInstance();

        inline GrpcAdpRpcServerThread& getGrpcServerThread()
        {
            return mRpcServerThread;
        }

        inline GrpcAdpRpcClientThread& getGrpcClientThread()
        {
            return mRpcClientThread;
        }

        GrpcAdpRpcClientThread* getRpcClientThread()
        {
            return &mRpcClientThread;
        }
        bool init();
        bool deinit();
        void join_threads();

    private:
        static GrpcAdpCntrl* mInstance;
        static int mInstId;
        int mThreadNum;
        std::vector<GrpcWorker *> mWorkerInstance;
        unsigned char mThreadInstanceIdGen;
        GrpcAdpRpcServerThread mRpcServerThread;
        GrpcAdpRpcClientThread mRpcClientThread;

        GrpcAdpCntrl();
        ~GrpcAdpCntrl() {}

        int createWorkerThreads();
        void joinAllThreads();

        int initGrpcServerConfig();
        int initGrpcClientConfig();
        int startGrpcServerThread(unsigned char& aThreadInstanceId);
        int startGrpcClientThread(unsigned char& aThreadInstanceId);
    };
} // namespace mav
