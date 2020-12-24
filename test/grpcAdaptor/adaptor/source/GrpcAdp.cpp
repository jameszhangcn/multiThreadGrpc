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
* File Name: CUUPLib.cpp
*
* Subsystem: libCUUP
*
* Description: This file defines the interface of libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher       Initial Release
*
**********************************************************************/

#include <cstdio>
#include <cstddef>
#include "SvcLog.h"
#include "GrpcAdpCntrl.h"
#include "GrpcAdp.h"

static int send_sec_cfg_count = 0;
static int send_sec_cfg_count_m = 0;

void mav_init()
{
    ccTrace();
    mav::GrpcAdpCntrl::getInstance()->init();
}

void init_grpc_server_config()
{
    ccTrace();
    printf("Init grpc server config");
}

void start_grpc_server()
{
    ccTrace("start_grpc_server");
    mav::GrpcAdpCntrl::getInstance()->init();
    printf("testing jianzhag");
    //mav::GrpcAdpCntrl::getInstance()->join_threads();
}

void create_grpc_client(char* cp_id, pthread_t tid, char* up_id)
{
    ccTrace("create_grpc_client");
    mav::GrpcAdpCntrl::getInstance()->create_client(cp_id, long(tid), up_id);
    printf(" create_grpc_client");
}

void join_grpc_server()
{
    ccTrace("join_grpc_server");
    mav::GrpcAdpCntrl::getInstance()->join_threads();
}

int send_sec_cfg_req(CuCpUpHdrG* cucp_up_hdr, SecurityConfigureG *secCfg)
{
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    send_sec_cfg_count++;
    if(send_sec_cfg_count != 1) {
        ccTrace("<<<<<<<<<<<<<<<<<<<<<<<< mutex >>>>>>>>>>>>>>>>> count %d ", send_sec_cfg_count);
    }
    int ret;
    ccTrace("Send sec cfg req: ");
    ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendSecurityCfg(cucp_up_hdr,secCfg);
    send_sec_cfg_count--;
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int send_sec_cfg_req_m(char * cpID, long cpInst, char* upID, CuCpUpHdrG* cucp_up_hdr, SecurityConfigureG *secCfg)
{
    send_sec_cfg_count_m++;
    if(send_sec_cfg_count_m != 1) {
        ccTrace("<<<<<<<<<<<<<<<<<<<<<<<< mutex >>>>>>>>>>>>>>>>> count %d ", send_sec_cfg_count_m);
    }
    EbmGrpcClient record {cpID, cpInst, upID};
    int ret;
    ccTrace("Send sec cfg req M: cp %s inst %x up %s ", cpID, cpInst, upID);

    if (false == mav::GrpcAdpCntrl::getInstance()->GrpcClientReady(record)) {
        ccTrace("GrpcClient not ready");//, Send sec cfg req M: cp %s inst %x up %s ", cpID, cpInst, upID);
        return 1;
    }
    ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread_M(record).SendSecurityCfg(cucp_up_hdr,secCfg);
    send_sec_cfg_count_m--;
    return ret;
}

