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
* File Name: CucpService.h
*
* Subsystem: Cucp
*
* Description: This file defines the service layer for Iwf
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 11/03/2020  JianZhang       Initial Release
*
**********************************************************************/
#pragma once

#include <unistd.h>
#include <grpcpp/grpcpp.h>
#include "structure.grpc.pb.h"
#include "srvEbm.grpc.pb.h"
#include "srvCp.grpc.pb.h"
#include "ServiceImpl.h"
#include "SvcLog.h"

using namespace srvEbm;
using namespace srvCp;

enum RpcType {

    // CUCP -> EBM
    SEC_CFG_REQ,
    
    // EBM -> CUCP
    SEC_CFG_CFM,
    TUN_CRT_CFM
};

DEF_RPC_CLIENT_OBJ(CucpEbmClient, SEC_CFG_REQ, Ebm, SecurityConfigure, SecurityConfigureReq, ReqReply);

DEF_RPC_SERVER_OBJ(EbmSecCfgCfmCallData, SEC_CFG_CFM, SecurityConfigureCfm, Cp, SecurityConfigureRsp, CfmReply);


class CucpServer : public ServiceServerImpl
{
public:
    ~CucpServer();

    virtual void OnInit();
    virtual grpc::Service* getService();
    virtual ServiceServerCallData* allocCallData(int aServiceType);
    virtual void deallocCallData(ServiceServerCallData* aCD);
    virtual void OnMessage(ServiceServerCallData& aCD);

private:
    virtual void OnSecCfgCfm(SecurityConfigureRsp& aInd);
    Cp::AsyncService mService;
};

class EbmClient : public ServiceClientImpl
{
};
