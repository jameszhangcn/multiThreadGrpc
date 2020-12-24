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
* File Name: CUUPLibRpcServerThread.cpp
*
* Subsystem: libCUUP
*
* Description: This file defines the RPC server for libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher       Initial Release
*
**********************************************************************/
#include "GrpcAdpRpcServerThread.h"
#include "GrpcAdpCntrl.h"
#include "EbmCfmCb.h"
#include "SvcLog.h"

void GrpcAdpRpcServerThread::init(uint32_t aThreadInstId)
{
    mThreadInstId = aThreadInstId;
    //CUUPLibRpcServerThread::Init(mIp + ":" + std::to_string(mPort));
    if(mServerAddr.size() > 0)
    {
        ccLog("init server %s", mServerAddr.c_str());
        CucpServer::Init(mServerAddr);
        mThreadReady = true;
    }
}

void GrpcAdpRpcServerThread::on_init()
{
    ccLog("ServerThread init");
}

void GrpcAdpRpcServerThread::on_exit()
{
    ccLog("ServerThread exit");
    mThreadInstId = 0;
    mThreadReady = false;
}

void GrpcAdpRpcServerThread::run()
{
    StartThread();
}

void GrpcAdpRpcServerThread::OnSecCfgCfm(SecurityConfigureRsp& aInd)
{
    ccLog("OnSecCfgCfm");
    //transfer the rsp to trillium
    printf("before send_sec_cfg_cfm");
    send_sec_cfg_cfm();
    printf("after send_sec_cfg_cfm");
    //update the route table for dummy_up
    CuCpUpHdrG cuCpUpHdr;
    update_route_table(&cuCpUpHdr);
    printf("update_route_table");
}