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
* File Name: CUUPLibRpcClientThread.cpp
*
* Subsystem: libCUUP
*
* Description: This file defines the RPC client for libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher Yu    Initial Release
*
**********************************************************************/
#include "GrpcAdpRpcClientThread.h"
#include "SvcLog.h"
#include "CucpService.h"

enum {
    SERVICE_EBM
};

void GrpcAdpRpcClientThread::init(uint32_t aThreadInstId)
{
    mThreadInstId = aThreadInstId;

    ccLog("Connect to EBM %s", mEbmServerTarget.c_str());
    printf("Connect to EBM %s", mEbmServerTarget.c_str());
    Connect(SERVICE_EBM, mEbmServerTarget);

    mThreadReady = true;
}

void GrpcAdpRpcClientThread::on_init()
{
    ccLog("");
}

int GrpcAdpRpcClientThread::SendSecurityCfg(CuCpUpHdrG* cucp_up_hdr, SecurityConfigureG *secCfgIn)
{
    static uint32_t seq = 0;
    srvEbm::SecurityConfigureReq aMsg;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    hdr->set_method(srvEbm::MethodSecurityConfigureReq);
    hdr->set_methodname("MethodSecurityConfigureReq");
    hdr->mutable_torid()->set_sn("EBM");
    hdr->mutable_fromrid()->set_sn("CUCP");
    hdr->mutable_fromloc()->set_address("127.0.0.1");
    hdr->mutable_fromloc()->set_port(9021);

    hdr->mutable_toloc()->set_address("127.0.0.1");
    hdr->mutable_toloc()->set_port(9027);

    hdr->set_transid(seq++);

    cpUpHdr->mutable_pst()->set_dst_inst(11);
    cpUpHdr->mutable_pst()->set_event(22);
    cpUpHdr->mutable_pst()->set_cell_id(33);

    cpUpHdr->mutable_cu_cp_id()->set_cp_id(5555);
    cpUpHdr->mutable_cu_cp_id()->set_cell_id(33);
    cpUpHdr->mutable_cu_cp_id()->set_ue_id(1234);
    cpUpHdr->mutable_cu_cp_id()->set_rb_id(0);

    cpUpHdr->mutable_cu_up_id()->set_up_id(8888);
    cpUpHdr->mutable_cu_up_id()->set_cell_id(0);
    cpUpHdr->mutable_cu_up_id()->set_ue_id(0);
    cpUpHdr->mutable_cu_up_id()->set_rb_id(0);

    cpUpHdr->mutable_su_sp_id()->set_su_id(6677);
    cpUpHdr->mutable_su_sp_id()->set_sp_id(7788);

    aMsg.mutable_security_config()->set_algorithm_type(secCfgIn->algorithm_type);

    ::structure::SecurityConfigure *secCfg = aMsg.mutable_security_config();///->mutable_encryption_key();
    google::protobuf::uint32* pkey;
    //pkey = secCfgKey.add_encryption_key();
    //pkey->set_encryption_key()
    
    for (int i=0; i < CPJ_CIPHER_KEY_LEN_G; i++) {
        //aMsg.mutable_security_config()->set_encryption_key(i,secCfg->encryption_key[i]);
        secCfg->add_encryption_key(secCfgIn->encryption_key[i]);

    }
    ccLog("Set encryption key");
    CucpEbmClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    client.Send(aMsg);
    return 0; // may return other failure code if needed
}

void GrpcAdpRpcClientThread::on_exit()
{
    ccLog("");
    mThreadInstId = 0;
    mThreadReady = false;
}

void GrpcAdpRpcClientThread::run()
{
    StartThread();
}

void GrpcAdpRpcClientThread::ResponseHandler(ServiceClientCallData* aClientCallData)
{
    ccLog("Rpc response handler");
    switch (aClientCallData->getRpcType())
    {
        case SEC_CFG_REQ: 
        {
            //TODO
            ccLog("get response, %d", aClientCallData->getRpcType());
        }
        break;
        default:
            ccLog("Unsupported type by now, %d", aClientCallData->getRpcType());
    }
}


