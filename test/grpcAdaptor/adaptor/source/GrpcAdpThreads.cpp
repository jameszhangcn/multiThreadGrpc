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
* File Name: CUUPLibThreads.cpp
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
#include "GrpcAdpThreads.h"
#include "SvcLog.h"

GrpcAdpThreads* GrpcAdpThreads::mInstance = nullptr;

GrpcAdpThreads::GrpcAdpThreads()
    : mThreadInstanceIdGen(1)
{
}

bool GrpcAdpThreads::startRpcServerThread(std::string aName)
{
    mRpcServerThread.init(mThreadInstanceIdGen++);

    mRpcServerThread.run();

    return true;
}

bool GrpcAdpThreads::startRpcClientThread(std::string aName)
{
    mRpcClientThread.init(mThreadInstanceIdGen++);

    mRpcClientThread.run();

    return true;
}

bool GrpcAdpThreads::joinRpcServerThread()
{
    mRpcServerThread.ThreadJoin();
    return true;
}

bool GrpcAdpThreads::joinRpcClientThread()
{
    mRpcClientThread.ThreadJoin();
    return true;
}

bool GrpcAdpThreads::killRpcServerThread()
{
    mRpcServerThread.Stop();
    return true;
}

bool GrpcAdpThreads::killRpcClientThread()
{
    mRpcClientThread.Stop();
    return true;
}
