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
#pragma once

#include <unistd.h>
#include <thread>
#include <netinet/sctp.h>
#include "ServiceProtobuf.h"

class ServiceClientCallData {
public:
    //Empty mReply;
    virtual ~ServiceClientCallData(){};
    grpc::ClientContext mContext;
    grpc::Status mStatus;
    virtual int getRpcType() = 0;
    //std::unique_ptr<grpc::ClientAsyncResponseReader<Empty>> mResponseReader;
};

#define DEF_RPC_CLIENT_OBJ(_OBJ_NAME, _RPC_TYPE, _SVC_NAME, _RPC_NAME, _RPC_REQ, _RPC_RSP) \
class Call_Data##_OBJ_NAME: public ServiceClientCallData {\
    public:\
        virtual ~Call_Data##_OBJ_NAME() {} \
        virtual int getRpcType(){return _RPC_TYPE;};\
        _RPC_RSP mReply; \
        std::unique_ptr<grpc::ClientAsyncResponseReader<_RPC_RSP>> mResponseReader; \
    };\
class _OBJ_NAME { \
public: \
    _OBJ_NAME(std::shared_ptr<grpc::Channel> aChannel, std::shared_ptr<grpc::CompletionQueue> aCq) \
        : m##_SVC_NAME(_SVC_NAME::NewStub(aChannel)) \
        , mCq(aCq) {} \
 \
    void Send(_RPC_REQ aMsg) \
    { \
        Call_Data##_OBJ_NAME* call = new Call_Data##_OBJ_NAME(); \
        call->mResponseReader = m##_SVC_NAME->PrepareAsync##_RPC_NAME(&call->mContext, aMsg, mCq.get()); \
        call->mResponseReader->StartCall(); \
        call->mResponseReader->Finish(&call->mReply, &call->mStatus, (void*)call); \
        return; \
    } \
 \
private: \
    std::unique_ptr<_SVC_NAME::Stub> m##_SVC_NAME; \
    std::shared_ptr<grpc::CompletionQueue> mCq; \
};

class ServiceClientImpl {
public:
    ServiceClientImpl();
    virtual ~ServiceClientImpl();

    std::shared_ptr<grpc::Channel> getChannel() {return mChannel[0];};
    std::shared_ptr<grpc::Channel> getChannel(int aId) {return mChannel[aId];};
    std::shared_ptr<grpc::CompletionQueue> getCompletionQueue() {return mCq;};

    void Connect(std::string aServerAddr);
    void Connect(int aId, std::string aServerAddr);
    void Run();
    virtual void StartThread();
    virtual void ThreadJoin();
    virtual void Stop();

    virtual void ResponseHandler(ServiceClientCallData* aClientCallData)=0;

private:
    void AsyncCompleteRpc();

#define MAX_SERVICE_CHANNEL 2
    std::shared_ptr<grpc::Channel> mChannel[MAX_SERVICE_CHANNEL];
    std::shared_ptr<grpc::CompletionQueue> mCq;
    bool mInit;
    std::thread mThread;
    bool mStop;
};

