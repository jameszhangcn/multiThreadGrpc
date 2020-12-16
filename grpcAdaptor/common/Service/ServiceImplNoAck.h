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
#include <iostream>
#include <netinet/sctp.h>
#include "ServiceProtobuf.h"

#define RPC_UNRELIABLE

#define DEF_RPC_CLIENT_OBJ(_OBJ_NAME, _SVC_NAME, _RPC_NAME, _RPC_MSG) \
class _OBJ_NAME { \
public: \
    _OBJ_NAME(std::shared_ptr<grpc::Channel> aChannel) \
        : m##_SVC_NAME(_SVC_NAME::NewStub(aChannel)) {} \
    ~_OBJ_NAME() {} \
 \
    void Send(_RPC_MSG aMsg) \
    { \
        mContext.reset(new grpc::ClientContext); \
        m##_SVC_NAME->PrepareAsync##_RPC_NAME(mContext.get(), aMsg, &mCq)->StartCall(); \
    } \
 \
private: \
    std::unique_ptr<grpc::ClientContext> mContext; \
    grpc::CompletionQueue mCq; \
    std::unique_ptr<_SVC_NAME::Stub> m##_SVC_NAME; \
};

#define DEF_RPC_SERVER_OBJ(_OBJ_NAME, _RPC_TYPE, _RPC_NAME, _SVC_NAME, _RPC_MSG) \
class _OBJ_NAME : public CallData { \
public: \
    _OBJ_NAME() \
        : mRequest(new _RPC_MSG) {} \
    virtual ~_OBJ_NAME() {} \
    virtual void ServiceRequest() { \
        dynamic_cast<_SVC_NAME::AsyncService*>(getService())->Request##_RPC_NAME( \
                getServerContext(), \
                mRequest.get(), \
                getResponder(), \
                getCompletionQueue(), \
                getServerCompletionQueue(), \
                this); \
    } \
    virtual int getRpcType() {return _RPC_TYPE;}; \
    virtual google::protobuf::Message* getMessage() {return mRequest.get();}; \
 \
private: \
    std::unique_ptr<_RPC_MSG> mRequest; \
};

class ServerImpl;
class CallData {
public:
    CallData();
    virtual ~CallData();

    void Register(ServerImpl& aServer);

    virtual void ServiceRequest() = 0;
    virtual int getRpcType() = 0;
    virtual google::protobuf::Message* getMessage() = 0;
    grpc::ServerContext* getServerContext() {return mCtx.get();};
    grpc::ServerCompletionQueue* getServerCompletionQueue() {return mCq;};
    grpc::CompletionQueue* getCompletionQueue() {return mCq;};
    grpc::ServerAsyncResponseWriter<Empty>* getResponder() {return &mResponder;};
    ServerImpl* getServer() {return mServer;};
    grpc::Service* getService();

    bool isToBeRelease() {return mToBeRelease;}
    void setToBeRelease() {mToBeRelease = true;}

    void Reset();

private:
    ServerImpl* mServer;
    grpc::ServerCompletionQueue* mCq;
    std::unique_ptr<grpc::ServerContext> mCtx;

    Empty mReply;
    grpc::ServerAsyncResponseWriter<Empty> mResponder;

    bool mToBeRelease;
};

class ServerImpl {
 public:
  virtual ~ServerImpl();

  virtual void OnInit() = 0;
  virtual grpc::Service* getService() = 0;
  virtual CallData* allocCallData(int aServiceType) = 0;
  virtual void deallocCallData(CallData* aCD) = 0;
  virtual void OnMessage(CallData& aCD) = 0;
  grpc::ServerCompletionQueue* getServerCompletionQueue() {return mCq.get();};

  // There is no shutdown handling in this code.
  void Init(const char* aAddr);
  void Run();
  void InitAndRun(const char* aAddr);
  void AddServiceHandler(int aServiceType);

 private:
  // This can be run in multiple threads if needed.
  void HandleRpcs();
  void OnNext(CallData* aCD);

  grpc::ServerBuilder mBuilder;
  std::unique_ptr<grpc::ServerCompletionQueue> mCq;
  std::unique_ptr<grpc::Server> mServer;
  std::string mServerAddress;
};

