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
* File Name: CUUPLibRpcClientThread.h
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
#pragma once

#include "CucpService.h"
#include "../../../include/GrpcAdpStru.h"
#include "../../../include/EbmCfmCb.h"
#include "structure.pb.h"
#include "srvEbm.pb.h"
#include "srvCp.pb.h"
#include "string.h"

class GrpcAdpRpcClientThread : public EbmClient
{
public:
    GrpcAdpRpcClientThread()
        : mThreadInstId(0)
        , mThreadReady(false)
    {
    }
    void init(uint32_t mThreadInstId);
    virtual void on_init();
    virtual void on_exit();
    virtual void run();

    inline uint32_t get_thread_inst_id()
    {
        return mThreadInstId;
    }

    void set_ebm_ip_port(std::string ebmID)
    {
        mEbmServerTarget = get_ebm_ip_port(ebmID.c_str());
    }

    inline std::string& ebmServerTarget() {return mEbmServerTarget;}
    int SendSecurityCfg(CuCpUpHdrG* cp_up_hdr, SecurityConfigureG *secCfg);
    void ResponseHandler(ServiceClientCallData* aClientCallData);
private:
    uint32_t mThreadInstId;
    bool mThreadReady;

    std::string mEbmServerTarget;
};
