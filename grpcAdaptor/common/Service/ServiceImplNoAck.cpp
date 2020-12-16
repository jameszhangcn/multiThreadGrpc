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
#include <iostream>
#include <netinet/sctp.h>
#include <grpcpp/grpcpp.h>
#include "SvcLog.h"
#include "ServiceImplNoAck.h"

CallData::CallData()
    : mCtx(new grpc::ServerContext)
    , mResponder(getServerContext())
    , mToBeRelease(false)
{
}

CallData::~CallData()
{
    mServer = nullptr;
    mCq = nullptr;
}

void CallData::Register(ServerImpl& aServer)
{
    mServer = &aServer;
    mCq = aServer.getServerCompletionQueue();
}

grpc::Service* CallData::getService()
{
    return mServer->getService();
}

ServerImpl::~ServerImpl()
{
    mServer->Shutdown();
    // Always shutdown the completion queue after the server.
    mCq->Shutdown();
}

void CallData::Reset()
{
    mCtx.reset(new grpc::ServerContext);
}

void ServerImpl::Init(const char* aAddr)
{
    mServerAddress = aAddr;

    // Listen on the given address without any authentication mechanism.
    mBuilder.AddListeningPort(mServerAddress, grpc::InsecureServerCredentials());
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    mBuilder.RegisterService(getService());
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    mCq = mBuilder.AddCompletionQueue();

    // Finally assemble the server.
    mServer = mBuilder.BuildAndStart();
    ccLog("Server listening on %s", mServerAddress.c_str());

    OnInit();
}

void ServerImpl::Run()
{
    // Proceed to the server's main loop.
    HandleRpcs();
}

void ServerImpl::InitAndRun(const char* aAddr)
{
    Init(aAddr);
    Run();
}

void ServerImpl::AddServiceHandler(int aServiceType)
{
    CallData* cd = allocCallData(aServiceType);
    cd->Register(*this);
    cd->ServiceRequest();
}

void ServerImpl::HandleRpcs()
{
    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
      if(!mCq->Next(&tag, &ok))
      {
        ccError("No Next");
      }
      if(!ok)
      {
        ccError("NOK");
      }
      OnNext(static_cast<CallData*>(tag));
    }
}

void ServerImpl::OnNext(CallData* aCD)
{
    // The actual processing.
    OnMessage(*aCD);

    // Waiting for the next RPC.
    aCD->Reset();
    aCD->ServiceRequest();
}

