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
#include "SvcLog.h"

#define DEF_RPC_SERVER_OBJ(_OBJ_NAME, _RPC_TYPE, _RPC_NAME, _SVC_NAME, _RPC_REQ, _RPC_RSP) \
class _OBJ_NAME : public ServiceServerCallData { \
public: \
    _OBJ_NAME() \
        : mRequest(new _RPC_REQ), mResponder(getServerContext()) {} \
    virtual ~_OBJ_NAME() {} \
    virtual void ServiceRequest() { \
        PrintServiceReq(mRequest.get(),getResponder()); \
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
    virtual void Finish(){ \
        ::mhdr::MsgHdr* hdr = &(const_cast<::mhdr::MsgHdr &>(mRequest.get()->hdr()));\
    	*mResponse.mutable_hdr() = *hdr; \
    	*mResponse.mutable_hdr()->mutable_torid() = const_cast<::mhdr::RID&>(const_cast<::mhdr::MsgHdr &>(mRequest.get()->hdr()).fromrid());\
    	*mResponse.mutable_hdr()->mutable_toloc() = const_cast<::mhdr::Location&>(const_cast<::mhdr::MsgHdr &>(mRequest.get()->hdr()).fromloc());\
    	*mResponse.mutable_hdr()->mutable_fromrid() = const_cast<::mhdr::RID&>(const_cast<::mhdr::MsgHdr &>(mRequest.get()->hdr()).torid());\
    	*mResponse.mutable_hdr()->mutable_fromloc() = const_cast<::mhdr::Location&>(const_cast<::mhdr::MsgHdr &>(mRequest.get()->hdr()).toloc());\
        mResponder.Finish(mResponse, grpc::Status::OK, this);\
        setToBeRelease();\
    }\
    grpc::ServerAsyncResponseWriter<_RPC_RSP>* getResponder() {return &mResponder;};\
    _RPC_RSP &getResponse(){return mResponse;}; \
 \
private: \
    std::unique_ptr<_RPC_REQ> mRequest; \
    _RPC_RSP mResponse; \
    grpc::ServerAsyncResponseWriter<_RPC_RSP> mResponder;\
};

class ServiceServerImpl;
class ServiceServerCallData {
public:
    ServiceServerCallData();
    virtual ~ServiceServerCallData();

    void Register(ServiceServerImpl& aServer);

    virtual void ServiceRequest() = 0;
    virtual int getRpcType() = 0;
    virtual google::protobuf::Message* getMessage() = 0;
    
    void PrintServiceReq(void * req, void * responder ){
        ccLog("getService() %x",getService());
        ccLog("getServerContext() %x",getServerContext());
        //ccLog("getResponder() %x",getResponder());
          ccLog("getCompletionQueue() %x",getCompletionQueue());
           ccLog("getServerCompletionQueue() %x",getServerCompletionQueue());
            ccLog("this() %x",this);
            ccLog("req() %x",req);
            ccLog("responder() %x",responder);
            return;
    };
    grpc::ServerContext* getServerContext() {
        printf("getServerContext");
        ccLog("getServerContext ");
        return &mCtx;
        };
    grpc::ServerCompletionQueue* getServerCompletionQueue() {
        printf("getServerCompletionQueue");
        ccLog("getServerCompletionQueue ");
        return mCq;};
    grpc::CompletionQueue* getCompletionQueue() {
        printf("getCompletionQueue");
        ccLog("getCompletionQueue ");
        return mCq;};
    //grpc::ServerAsyncResponseWriter<Empty>* getResponder() {return &mResponder;};
    ServiceServerImpl* getServer() {
        printf("getServer");
        ccLog("getServer ");
        return mServer;};
    grpc::Service* getService();
    virtual void Finish() = 0;

    bool isToBeRelease() {return mToBeRelease;}
    void setToBeRelease() {mToBeRelease = true;}

private:
    ServiceServerImpl* mServer;
    grpc::ServerCompletionQueue* mCq;
    grpc::ServerContext mCtx;

    //Empty mReply;
    //grpc::ServerAsyncResponseWriter<Empty> mResponder;

    bool mToBeRelease;
};

class ServiceServerImpl {
 public:
  ServiceServerImpl();
  virtual ~ServiceServerImpl();

  virtual void StartThread();
  virtual void ThreadJoin();
  virtual void Stop();
  virtual void OnInit() = 0;
  virtual grpc::Service* getService() = 0;
  virtual ServiceServerCallData* allocCallData(int aServiceType) = 0;
  virtual void deallocCallData(ServiceServerCallData* aCD) = 0;
  virtual void OnMessage(ServiceServerCallData& aCD) = 0;
  grpc::ServerCompletionQueue* getServerCompletionQueue() {return mCq.get();};

  // There is no shutdown handling in this code.
  void Init(std::string aAddr);
  void Run();
  void InitAndRun(std::string aAddr);
  void AddServiceHandler(int aServiceType);

 private:
  // This can be run in multiple threads if needed.
  void HandleRpcs();
  void OnNext(ServiceServerCallData* aCD);

  grpc::ServerBuilder mBuilder;
  std::unique_ptr<grpc::ServerCompletionQueue> mCq;
  std::unique_ptr<grpc::Server> mServer;
  std::string mServerAddress;
  bool mInit;
  std::thread mThread;
  bool mStop;
};

