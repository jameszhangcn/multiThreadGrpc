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
* File Name: CUUPLibRpcServerThread.h
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
#pragma once

#include "CucpService.h"

class GrpcAdpRpcServerThread : public CucpServer
{
public:
    GrpcAdpRpcServerThread()
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
    //inline std::string& IP() {return mIp;};
    //inline uint16_t& Port() {return mPort;};
    inline std::string & serverAddr(){return mServerAddr;}
    virtual void OnSecCfgCfm(SecurityConfigureRsp& aInd);

private:
    uint32_t mThreadInstId;
    bool mThreadReady;

    //std::string mIp;
    //uint16_t mPort;
    std::string mServerAddr;
};
