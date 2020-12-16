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
* File Name: CUUPLibThreads.h
*
* Subsystem: libCUUP
*
* Description: This file defines the threads for libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher       Initial Release
*
**********************************************************************/
#pragma once

#include "GrpcAdpRpcServerThread.h"
#include "GrpcAdpRpcClientThread.h"


class GrpcAdpThreads
{
public:
    static inline GrpcAdpThreads* getInstance()
    {
        if (mInstance == nullptr)
        {
            mInstance = new GrpcAdpThreads;
        }

        return mInstance;
    }

    static inline void resetInstance()
    {
        delete mInstance;
        mInstance = 0;
    }

    bool startRpcServerThread(std::string aName);
    bool startRpcClientThread(std::string aName);

    bool joinRpcServerThread();
    bool joinRpcClientThread();

    bool killRpcServerThread();
    bool killRpcClientThread();

    inline GrpcAdpRpcServerThread& getRpcServerThread()
    {
        return mRpcServerThread;
    }
    inline GrpcAdpRpcClientThread& getRpcClientThread()
    {
        return mRpcClientThread;
    }

private:
    static GrpcAdpThreads* mInstance;
    GrpcAdpThreads();

    uint32_t mThreadInstanceIdGen; // Running number

    // Single-instance threads
    GrpcAdpRpcServerThread mRpcServerThread;
    GrpcAdpRpcClientThread mRpcClientThread;
};
