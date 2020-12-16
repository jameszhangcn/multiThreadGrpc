/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2004-2019                                           */
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
* File Name: ServiceImpl.h
*
* Subsystem: gRPC Service
*
* Description: This file defines the service layer
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 11/04/2019  Fisher       Initial Release
*
**********************************************************************/
#include <unistd.h>
#include <netinet/sctp.h>
#include <grpcpp/grpcpp.h>
#include "SvcLog.h"
#include "ServiceClientImpl.h"

ServiceClientImpl::ServiceClientImpl()
    : mInit(false)
    , mStop(false)
{
}

ServiceClientImpl::~ServiceClientImpl()
{
}

void ServiceClientImpl::Connect(std::string aServerAddr)
{
    mChannel[0] = grpc::CreateChannel(aServerAddr, grpc::InsecureChannelCredentials());
    if(!mCq)
    {
        mCq = std::make_shared<grpc::CompletionQueue>();
    }
    mInit = true;
}

void ServiceClientImpl::Connect(int aId, std::string aServerAddr)
{
    mChannel[aId] = grpc::CreateChannel(aServerAddr, grpc::InsecureChannelCredentials());
    if(!mCq)
    {
        mCq = std::make_shared<grpc::CompletionQueue>();
    }
    mInit = true;
}

void ServiceClientImpl::Run()
{
    // Proceed to the client's main loop
    AsyncCompleteRpc();
}

void ServiceClientImpl::StartThread()
{
    if(!mInit)
    {
        ccError("Please init the client before start");
        return;
    }

    mThread = std::thread(&ServiceClientImpl::Run, this);
}

void ServiceClientImpl::ThreadJoin()
{
    mThread.join();
}

void ServiceClientImpl::Stop()
{
    mStop = true;
}

void ServiceClientImpl::AsyncCompleteRpc()
{
    void* got_tag;
    bool ok = false;

    while (mCq->Next(&got_tag, &ok)) {
        ServiceClientCallData* call = static_cast<ServiceClientCallData*>(got_tag);
        if(!ok)
            ccError("Not ok");

        if (call->mStatus.ok())
        {
            ccLog("Reply received");
            ResponseHandler(call);
        }
        else
            ccError("RPC failed");

        delete call;

        if(mStop)
        {
            ccError("Stop!");
            break;
        }
    }
}

