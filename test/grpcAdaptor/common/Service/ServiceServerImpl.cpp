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
#include "ServiceServerImpl.h"

ServiceServerCallData::ServiceServerCallData()
    : mToBeRelease(false)
{
}

ServiceServerCallData::~ServiceServerCallData()
{
    mServer = nullptr;
    mCq = nullptr;
}

void ServiceServerCallData::Register(ServiceServerImpl& aServer)
{
    mServer = &aServer;
    mCq = aServer.getServerCompletionQueue();
}

grpc::Service* ServiceServerCallData::getService()
{
    return mServer->getService();
}
#if 0
void ServiceServerCallData::Finish()
{
    mResponder.Finish(mReply, grpc::Status::OK, this);
    setToBeRelease();
}
#endif

ServiceServerImpl::ServiceServerImpl()
    : mInit(false)
    , mStop(false)
{
}

ServiceServerImpl::~ServiceServerImpl()
{
    mServer->Shutdown();
    // Always shutdown the completion queue after the server.
    mCq->Shutdown();
}

void ServiceServerImpl::StartThread()
{
    if(!mInit)
    {
        ccError("Please init the server before start");
        return;
    }

    mThread = std::thread(&ServiceServerImpl::Run, this);
}

void ServiceServerImpl::ThreadJoin()
{
    mThread.join();
}

void ServiceServerImpl::Stop()
{
    mStop = true;
}

void ServiceServerImpl::Init(std::string aAddr)
{
    mServerAddress = aAddr;

    // Listen on the given address without any authentication mechanism.
    mBuilder.AddListeningPort(mServerAddress, grpc::InsecureServerCredentials());
    ccLog("ServiceServerImpl Add Listening port ");
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    mBuilder.RegisterService(getService());
    ccLog("ServiceServerImpl Register service port ");
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    mCq = mBuilder.AddCompletionQueue();
    ccLog("ServiceServerImpl AddCompletionQueue ");
    // Finally assemble the server.
    mServer = mBuilder.BuildAndStart();
    ccLog(" ServiceServerImpl::Init BuildAndStart mServer %x ", mServer.get());
    ccLog("Server listening on %s", mServerAddress.c_str());

    OnInit();
    ccLog("ServiceServerImpl OnInit ");

    mInit = true;
}

void ServiceServerImpl::Run()
{
    // Proceed to the server's main loop.
    HandleRpcs();
}

void ServiceServerImpl::InitAndRun(std::string aAddr)
{
    Init(aAddr);
    Run();
}

void ServiceServerImpl::AddServiceHandler(int aServiceType)
{
    ccLog("AddServiceHandler %d ", aServiceType);
    ServiceServerCallData* cd = allocCallData(aServiceType);
    cd->Register(*this);
    ccLog("AddServiceHandler-Register");
    cd->ServiceRequest();
    ccLog("AddServiceHandler-ServiceRequest");
}

void ServiceServerImpl::HandleRpcs()
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

      OnNext(static_cast<ServiceServerCallData*>(tag));

      if(mStop)
      {
          ccError("Stop!");
          break;
      }
    }
}

void ServiceServerImpl::OnNext(ServiceServerCallData* aCD)
{
    if(aCD->isToBeRelease())
    {
        deallocCallData(aCD);
        return;
    }

    // Spawn a new CallData instance to serve new clients while we process
    // the one for this CallData. The instance will deallocate itself as
    // part of its FINISH state.
    AddServiceHandler(aCD->getRpcType());

    // The actual processing.
    OnMessage(*aCD);

    aCD->Finish();
}

